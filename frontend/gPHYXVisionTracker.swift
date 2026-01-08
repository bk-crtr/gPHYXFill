import Foundation
import Vision
import CoreVideo

@objc(gPHYXVisionTracker)
public class gPHYXVisionTracker: NSObject {
    
    private var sequenceHandler = VNSequenceRequestHandler()
    private var registrationHandler = VNSequenceRequestHandler()
    
    @objc public func reset() {
        sequenceHandler = VNSequenceRequestHandler()
        registrationHandler = VNSequenceRequestHandler()
    }
    
    @objc public func estimateHomography(from sourceBuffer: CVPixelBuffer, to referenceBuffer: CVPixelBuffer, roi: CGRect) -> [Float] {
        let request = VNHomographicImageRegistrationRequest(targetedCVPixelBuffer: referenceBuffer)
        
        // Apply MOCHA-style ROI (normalized 0..1)
        // Ensure ROI is valid (non-empty)
        if roi.width > 0 && roi.height > 0 {
            request.regionOfInterest = roi
        }
        
        do {
            try registrationHandler.perform([request], on: sourceBuffer)
            if let observation = request.results?.first as? VNImageHomographicAlignmentObservation {
                let matrix = observation.warpTransform
                // matrix is simd_float3x3. Convert to [Float]
                return [
                    matrix.columns.0.x, matrix.columns.0.y, matrix.columns.0.z,
                    matrix.columns.1.x, matrix.columns.1.y, matrix.columns.1.z,
                    matrix.columns.2.x, matrix.columns.2.y, matrix.columns.2.z
                ]
            }
        } catch {
            print("[gPHYX] Vision Registration Error: \(error)")
        }
        return [1, 0, 0, 0, 1, 0, 0, 0, 1] // Identity
    }
}
