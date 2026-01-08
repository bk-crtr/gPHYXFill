import cv2
import numpy as np
from fastapi import FastAPI, UploadFile, File, Form, HTTPException, Response
from pydantic import BaseModel
from typing import List, Optional
import json
import io

app = FastAPI()

class TrackingSession:
    def __init__(self):
        self.sift = cv2.SIFT_create()
        self.matcher = cv2.BFMatcher()
        
        self.ref_img = None
        self.ref_kp = None
        self.ref_des = None
        self.ref_pts = None
        
    def reset(self, frame, mask_points):
        """Initialize tracking with SIFT features inside a mask."""
        self.ref_img = frame
        
        # Create mask
        h, w = frame.shape[:2]
        mask = np.zeros((h, w), dtype=np.uint8)
        pts = np.array(mask_points, dtype=np.int32)
        cv2.fillPoly(mask, [pts], 255)
        
        # Detect SIFT features inside mask
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        self.ref_kp, self.ref_des = self.sift.detectAndCompute(gray, mask)
        
        # Store normalized points for tracking quality check
        if self.ref_kp:
            self.ref_pts = np.float32([kp.pt for kp in self.ref_kp]).reshape(-1, 1, 2)
        else:
            self.ref_pts = None
            
        print(f"Tracking initialized. SIFT Keypoints: {len(self.ref_kp) if self.ref_kp else 0}")

    def track(self, current_frame):
        """Perform SIFT matching and compute Homography."""
        if self.ref_des is None or len(self.ref_des) < 4:
            return np.eye(3).tolist()
            
        gray_curr = cv2.cvtColor(current_frame, cv2.COLOR_BGR2GRAY)
        curr_kp, curr_des = self.sift.detectAndCompute(gray_curr, None)
        
        if curr_des is None or len(curr_des) < 4:
            return np.eye(3).tolist()
            
        # Match descriptors
        matches = self.matcher.knnMatch(self.ref_des, curr_des, k=2)
        
        # Apply Lowe's ratio test
        good_matches = []
        for m, n in matches:
            if m.distance < 0.75 * n.distance:
                good_matches.append(m)
                
        if len(good_matches) >= 4:
            src_pts = np.float32([self.ref_kp[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
            dst_pts = np.float32([curr_kp[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)
            
            # Find Homography
            H, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
            if H is not None:
                return H.tolist()
                
        # Fallback to Identity
        return np.eye(3).tolist()

# Global session
session = TrackingSession()

@app.post("/init_track")
async def init_track(
    file: UploadFile = File(...),
    points_json: str = Form(...)
):
    try:
        contents = await file.read()
        nparr = np.frombuffer(contents, np.uint8)
        frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        
        if frame is None:
            raise HTTPException(status_code=400, detail="Invalid image data")
            
        points = json.loads(points_json)
        h, w = frame.shape[:2]
        
        # Convert normalized [0..1] points to pixel coordinates
        # FCP (OpenGL-style): [0,0] is bottom-left. CV2: [0,0] is top-left.
        pixel_points = [[p[0]*w, (1.0 - p[1])*h] for p in points]
        
        session.reset(frame, pixel_points)
        count = len(session.ref_kp) if session.ref_kp else 0
        return {"status": "success", "points_count": count}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/track_frame")
async def track_frame(file: UploadFile = File(...)):
    try:
        contents = await file.read()
        nparr = np.frombuffer(contents, np.uint8)
        frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        
        if frame is None:
            return {"homography": np.eye(3).tolist()}
            
        matrix = session.track(frame)
        return {"homography": matrix}
    except Exception as e:
        # In tracking, we prefer returning identity over crashing the host
        return {"homography": np.eye(3).tolist(), "error": str(e)}

@app.post("/inpaint")
async def inpaint(
    file: UploadFile = File(...),
    points_json: str = Form(...) # Reuse mask points for simplicity
):
    try:
        contents = await file.read()
        nparr = np.frombuffer(contents, np.uint8)
        frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        
        if frame is None:
             raise HTTPException(status_code=400, detail="Invalid image")
             
        # Create mask from points
        points = json.loads(points_json)
        h, w = frame.shape[:2]
        mask = np.zeros((h, w), dtype=np.uint8)
        pixel_points = [[p[0]*w, (1.0 - p[1])*h] for p in points]
        pts = np.array(pixel_points, dtype=np.int32)
        cv2.fillPoly(mask, [pts], 255)
        
        # Perform inpainting (Telea method as placeholder)
        # Using a small radius for better results in high-res
        result = cv2.inpaint(frame, mask, 3, cv2.INPAINT_TELEA)
        
        # Encode back to JPEG
        _, buffer = cv2.imencode(".jpg", result)
        return Response(content=buffer.tobytes(), media_type="image/jpeg")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/status")
async def get_status():
    return {"initialized": session.ref_img is not None}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8989)
