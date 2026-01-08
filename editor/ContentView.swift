import SwiftUI
import AVFoundation
import Vision
import Combine

class EditorViewModel: ObservableObject {
    @Published var points: [CGPoint] = []
    @Published var isTracking: Bool = false
    @Published var statusText: String = ""
    @Published var videoSize: CGSize = .zero
    @Published var currentTime: CMTime = .zero
    @Published var duration: CMTime = .zero
    @Published var draggedPointIndex: Int? = nil
    
    let videoOutput = AVPlayerItemVideoOutput(pixelBufferAttributes: [kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_32BGRA])
    var player: AVPlayer?
    @Published var videoPath: String = ""
    @Published var instanceID: String = ""
    @Published var canvasSize: CGSize = .zero
    
    var undoManager: UndoManager?
    private var trackingTask: Task<Void, Never>?
    
    func log(_ message: String) {
        let timestamp = Date().formatted(.dateTime.hour().minute().second())
        print("[\(timestamp)] [gPHYX Editor] \(message)")
    }
    
    func setup(videoPath: String, instanceID: String) {
        log("Setup called with path: \(videoPath), instance: \(instanceID)")
        self.videoPath = videoPath
        self.instanceID = instanceID
        
        if videoPath.isEmpty {
            log("⚠️ Video path is EMPTY")
            statusText = "Waiting for video path..."
            return
        }
        
        let url = URL(fileURLWithPath: videoPath)
        if FileManager.default.fileExists(atPath: videoPath) {
            let asset = AVAsset(url: url)
            let item = AVPlayerItem(asset: asset)
            item.add(videoOutput)
            player = AVPlayer(playerItem: item)
            
            Task {
                if let track = try? await asset.loadTracks(withMediaType: .video).first {
                    let size = try await track.load(.naturalSize)
                    let dur = try await asset.load(.duration)
                    await MainActor.run {
                        self.videoSize = size
                        self.duration = dur
                    }
                }
            }
            
            Timer.scheduledTimer(withTimeInterval: 0.1, repeats: true) { _ in
                if let player = self.player {
                    DispatchQueue.main.async {
                        self.currentTime = player.currentTime()
                    }
                }
            }
            
            statusText = "Video loaded: \(url.lastPathComponent)"
        } else {
            statusText = "Video not found at path"
        }
    }
    
    var videoRect: CGRect {
        guard videoSize != .zero, canvasSize != .zero else { return .zero }
        let aspect = videoSize.width / videoSize.height
        let canvasAspect = canvasSize.width / canvasSize.height
        
        if canvasAspect > aspect {
            let width = canvasSize.height * aspect
            return CGRect(x: (canvasSize.width - width) / 2, y: 0, width: width, height: canvasSize.height)
        } else {
            let height = canvasSize.width / aspect
            return CGRect(x: 0, y: (canvasSize.height - height) / 2, width: canvasSize.width, height: height)
        }
    }
    
    func seek(to seconds: Double) {
        let time = CMTime(seconds: seconds, preferredTimescale: 600)
        player?.seek(to: time, toleranceBefore: .zero, toleranceAfter: .zero)
    }
    
    func addPoint(_ pt: CGPoint) {
        let oldPoints = points
        undoManager?.registerUndo(withTarget: self) { target in
            target.updatePoints(oldPoints)
        }
        points.append(pt)
    }
    
    func movePoint(at index: Int, to newLocation: CGPoint) {
        guard index < points.count else { return }
        points[index] = newLocation
    }
    
    func commitMove(at index: Int, from oldLocation: CGPoint) {
        guard index < points.count else { return }
        let newLocation = points[index]
        undoManager?.registerUndo(withTarget: self) { target in
            target.points[index] = oldLocation
            target.commitMove(at: index, from: newLocation) // Recursive for redo
        }
    }

    func updatePoints(_ newPoints: [CGPoint]) {
        let oldPoints = points
        undoManager?.registerUndo(withTarget: self) { target in
            target.updatePoints(oldPoints)
        }
        points = newPoints
    }
    
    func deletePoint(at index: Int) {
        guard index >= 0 && index < points.count else { return }
        let oldPoints = points
        undoManager?.registerUndo(withTarget: self) { target in
            target.updatePoints(oldPoints)
        }
        points.remove(at: index)
    }
    
    func stopTracking() {
        trackingTask?.cancel()
        trackingTask = nil
        isTracking = false
        statusText = "⏹️ Stopped"
        log("Tracking stopped manually")
    }

    func trackStep(direction: Int) {
        guard let player = player, !points.isEmpty else {
            log("⚠️ Cannot track: player=\(player != nil), points.count=\(points.count)")
            return
        }
        log("🎯 === TRACKING STEP \(direction > 0 ? "FORWARD" : "BACKWARD") ===")
        log("Current points count: \(points.count)")
        
        let currentTime = player.currentTime()
        let currentSeconds = CMTimeGetSeconds(currentTime)
        log("Current time: \(String(format: "%.3f", currentSeconds))s")
        
        guard let pixelBuffer = videoOutput.copyPixelBuffer(forItemTime: currentTime, itemTimeForDisplay: nil) else {
            log("❌ FAILED: Could not get pixel buffer for current frame")
            statusText = "❌ No buffer"
            return
        }
        log("✅ Got pixel buffer for current frame")
        
        let rect = videoRect
        log("Video rect: \(rect)")
        var requests: [VNTrackObjectRequest] = []
        
        for (idx, pt) in points.enumerated() {
            let nx = (pt.x - rect.origin.x) / rect.width
            let ny = 1.0 - (pt.y - rect.origin.y) / rect.height
            log("Point[\(idx)]: canvas=(\(String(format: "%.1f", pt.x)), \(String(format: "%.1f", pt.y))) → normalized=(\(String(format: "%.3f", nx)), \(String(format: "%.3f", ny)))")
            
            let roiSize: CGFloat = 0.12 // Increased for better stickiness
            let roi = CGRect(x: clamp(nx - roiSize/2, 0, 1-roiSize), 
                             y: clamp(ny - roiSize/2, 0, 1-roiSize), 
                             width: roiSize, height: roiSize)
            log("  ROI[\(idx)]: x=\(String(format: "%.3f", roi.origin.x)), y=\(String(format: "%.3f", roi.origin.y)), size=\(String(format: "%.3f", roiSize))")
            
            let req = VNTrackObjectRequest(detectedObjectObservation: VNDetectedObjectObservation(boundingBox: roi))
            req.trackingLevel = .accurate
            requests.append(req)
        }
        
        let frameDuration = CMTime(value: Int64(direction * 1001), timescale: 24000)
        let nextTime = CMTimeAdd(currentTime, frameDuration)
        let nextSeconds = CMTimeGetSeconds(nextTime)
        log("Seeking to next frame: \(String(format: "%.3f", nextSeconds))s (delta: \(String(format: "%.3f", nextSeconds - currentSeconds))s)")
        
        player.seek(to: nextTime, toleranceBefore: .zero, toleranceAfter: .zero) { finished in
            self.log("Seek finished: \(finished)")
            // Small delay to let the output catch up
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.03) {
                guard let nextBuffer = self.videoOutput.copyPixelBuffer(forItemTime: nextTime, itemTimeForDisplay: nil) else {
                    self.log("❌ FAILED: Could not get pixel buffer for next frame (likely end/start of video)")
                    self.statusText = direction > 0 ? "🏁 End" : "⏪ Start"
                    return
                }
                self.log("✅ Got pixel buffer for next frame")
                
                let sequenceHandler = VNSequenceRequestHandler()
                do {
                    self.log("🔍 Performing Vision tracking requests...")
                    try sequenceHandler.perform(requests, on: nextBuffer)
                    var newPoints: [CGPoint] = []
                    var successCount = 0
                    var failCount = 0
                    
                    for (idx, req) in requests.enumerated() {
                        if let obs = req.results?.first as? VNDetectedObjectObservation {
                            self.log("  Result[\(idx)]: confidence=\(String(format: "%.3f", obs.confidence))")
                            if obs.confidence > 0.3 {
                                let bbox = obs.boundingBox
                                let px = rect.origin.x + (bbox.midX * rect.width)
                                let py = rect.origin.y + ((1.0 - bbox.midY) * rect.height)
                                newPoints.append(CGPoint(x: px, y: py))
                                successCount += 1
                                self.log("    Point[\(idx)] tracked: normalized bbox=(\(String(format: "%.3f", bbox.minX)), \(String(format: "%.3f", bbox.minY)), \(String(format: "%.3f", bbox.width)), \(String(format: "%.3f", bbox.height))) → canvas=(\(String(format: "%.1f", px)), \(String(format: "%.1f", py)))")
                            } else {
                                newPoints.append(self.points[idx])
                                failCount += 1
                                self.log("    Point[\(idx)] tracking failed (low confidence), retaining old position.")
                            }
                        } else {
                            newPoints.append(self.points[idx])
                            failCount += 1
                            self.log("    Point[\(idx)] tracking failed (no observation), retaining old position.")
                        }
                    }
                    self.points = newPoints // Direct update for tracking loop
                    self.currentTime = nextTime
                    self.statusText = "✅ Tracked \(successCount)/\(self.points.count)"
                    self.log("📊 Tracking complete: \(successCount) successful, \(failCount) failed")
                    self.log("🎯 === END TRACKING STEP ===\n")
                } catch {
                    self.log("❌ ERROR: Vision tracking failed: \(error.localizedDescription)")
                    self.statusText = "❌ Track error"
                }
            }
        }
    }

    func trackForward() {
        if isTracking { stopTracking(); return }
        isTracking = true
        statusText = "Tracking ⏭️..."
        
        trackingTask = Task {
            while isTracking {
                await MainActor.run { trackStep(direction: 1) }
                try? await Task.sleep(nanoseconds: 100_000_000) // 0.1s
                
                if let player = player, CMTimeCompare(player.currentTime(), duration) >= 0 {
                    await MainActor.run { 
                        isTracking = false 
                        statusText = "🏁 Finished Forward"
                    }
                    break
                }
            }
        }
    }
    
    func trackBackward() {
        if isTracking { stopTracking(); return }
        isTracking = true
        statusText = "Tracking ⏮️..."
        
        trackingTask = Task {
            while isTracking {
                await MainActor.run { trackStep(direction: -1) }
                try? await Task.sleep(nanoseconds: 100_000_000)
                
                if let player = player, player.currentTime().seconds <= 0.01 {
                    await MainActor.run { 
                        isTracking = false 
                        statusText = "🏁 Finished Backward"
                    }
                    break
                }
            }
        }
    }
    
    func trackFullAuto() {
        Task {
            log("🚀 Auto Track: Forward then Backward")
            await MainActor.run { trackForward() }
            while isTracking { try? await Task.sleep(nanoseconds: 500_000_000) }
            await MainActor.run { trackBackward() }
        }
    }
    
    func trackFrame(direction: Int) {
        guard let player = player else { return }
        let nextTime = CMTimeAdd(player.currentTime(), CMTime(value: Int64(direction * 1001), timescale: 24000))
        seek(to: nextTime.seconds)
    }
    
    private func clamp(_ val: CGFloat, _ minV: CGFloat, _ maxV: CGFloat) -> CGFloat {
        return min(max(val, minV), maxV)
    }
    
    func saveAndExit() {
        let jsonPath = "/tmp/gPHYX_mask_\(instanceID).json"
        let rect = videoRect
        guard rect.width > 0, rect.height > 0 else { return }
        
        let pointDicts = points.map { pt -> [String: Any] in
            let nx = (pt.x - rect.origin.x) / rect.width
            let ny = (pt.y - rect.origin.y) / rect.height
            return ["x": Double(nx), "y": Double(ny)]
        }
        
        let data: [String: Any] = [
            "instanceID": instanceID,
            "points": pointDicts,
            "timestamp": Date().timeIntervalSince1970
        ]
        
        if let jsonData = try? JSONSerialization.data(withJSONObject: data, options: .prettyPrinted) {
            try? jsonData.write(to: URL(fileURLWithPath: jsonPath))
        }
        NSApplication.shared.terminate(nil)
    }
}

struct ContentView: View {
    var videoPath: String
    var instanceID: String
    
    @StateObject private var viewModel = EditorViewModel()
    @Environment(\.undoManager) var undoManager
    
    var body: some View {
        VStack {
            HStack {
                VStack(alignment: .leading) {
                    Text("gPHYX Editor v1.2").font(.headline)
                    Text("Instance: \(instanceID)").font(.caption).foregroundColor(.gray)
                }.padding(.horizontal)
                Spacer()
                
                Button(action: { undoManager?.undo() }) { Image(systemName: "arrow.uturn.backward") }
                    .keyboardShortcut("z", modifiers: .command).help("Undo")
                
                Button(action: { viewModel.updatePoints([]) }) { Image(systemName: "trash") }
                    .padding(.trailing, 10).help("Clear")
                
                Button(action: { viewModel.trackFrame(direction: -1) }) { Image(systemName: "chevron.backward.2") }.help("Step Backward")
                Button(action: { viewModel.trackFrame(direction: 1) }) { Image(systemName: "chevron.forward.2") }.padding(.trailing, 5).help("Step Forward")
                
                Button(action: { viewModel.trackStep(direction: -1) }) { Image(systemName: "backward.frame") }.help("Track 1 Frame Backward")
                Button(action: { viewModel.trackStep(direction: 1) }) { Image(systemName: "forward.frame") }.help("Track 1 Frame Forward").padding(.trailing, 5)
                
                Button(action: { viewModel.trackBackward() }) { Image(systemName: "backward.circle.fill") }.help("Track Backward")
                Button(action: { viewModel.trackForward() }) { Image(systemName: "forward.circle.fill") }.help("Track Forward").padding(.trailing, 1)
                Button(action: { viewModel.trackFullAuto() }) { Label("Auto", systemImage: "wand.and.stars") }.help("Full Track Auto")
                
                if viewModel.isTracking {
                    Button(action: { viewModel.stopTracking() }) {
                        Label("Stop", systemImage: "stop.fill")
                            .foregroundColor(.white)
                            .padding(5)
                            .background(Color.red)
                            .cornerRadius(5)
                    }
                }
                
                Button("Save and Close") { viewModel.saveAndExit() }
                    .padding().keyboardShortcut("s", modifiers: .command)
            }.background(Color.gray.opacity(0.1))
            
            HStack {
                Text(timeString(from: viewModel.currentTime)).font(.caption.monospaced())
                Slider(value: Binding(get: { viewModel.currentTime.seconds }, set: { viewModel.seek(to: $0) }),
                       in: 0...(viewModel.duration.seconds > 0 ? viewModel.duration.seconds : 1))
                .accentColor(.pink)
                Text(timeString(from: viewModel.duration)).font(.caption.monospaced())
            }.padding(.horizontal).padding(.bottom, 5)
            
            ZStack {
                if let player = viewModel.player {
                    VideoPlayer(player: player)
                        .overlay(ZKeyframeView(viewModel: viewModel))
                } else {
                    Color.black.overlay(Text("No Video").foregroundColor(.white))
                }
            }
            .overlay(
                Text(viewModel.statusText).foregroundColor(.white).padding(8).background(Color.black.opacity(0.5)).cornerRadius(8).padding(),
                alignment: .bottom
            )
        }
        .frame(minWidth: 900, minHeight: 700)
        .onAppear {
            viewModel.undoManager = undoManager
            viewModel.setup(videoPath: videoPath, instanceID: instanceID)
        }
        .onChange(of: videoPath) { newPath in
            viewModel.setup(videoPath: newPath, instanceID: instanceID)
        }
        .onChange(of: instanceID) { newID in
            viewModel.setup(videoPath: videoPath, instanceID: newID)
        }
    }
    
    func timeString(from time: CMTime) -> String {
        let totalSeconds = Int(max(0, time.seconds))
        return String(format: "%02d:%02d", totalSeconds / 60, totalSeconds % 60)
    }
}

struct ZKeyframeView: View {
    @ObservedObject var viewModel: EditorViewModel
    
    var body: some View {
        GeometryReader { geo in
            Canvas { context, size in
                let rect = viewModel.videoRect
                context.stroke(Path(rect), with: .color(.white.opacity(0.2)), lineWidth: 1)
                
                if viewModel.points.count > 1 {
                    var path = Path()
                    path.move(to: viewModel.points[0])
                    for i in 1..<viewModel.points.count { path.addLine(to: viewModel.points[i]) }
                    path.closeSubpath()
                    context.stroke(path, with: .color(.pink), lineWidth: 2)
                    context.fill(path, with: .color(.pink.opacity(0.2)))
                }
                
                for (idx, pt) in viewModel.points.enumerated() {
                    let isDragged = viewModel.draggedPointIndex == idx
                    context.fill(Path(ellipseIn: CGRect(x: pt.x-5, y: pt.y-5, width: 10, height: 10)), 
                                 with: .color(isDragged ? .green : .blue))
                }
            }
            .background(Color.black.opacity(0.001))
            .onAppear { viewModel.canvasSize = geo.size }
            .onChange(of: geo.size) { viewModel.canvasSize = $0 }
            .gesture(
                DragGesture(minimumDistance: 0)
                    .onChanged { value in
                        if let idx = viewModel.draggedPointIndex {
                            viewModel.movePoint(at: idx, to: value.location)
                        } else if let foundIdx = viewModel.points.firstIndex(where: { hypot($0.x - value.startLocation.x, $0.y - value.startLocation.y) < 20 }) {
                            viewModel.draggedPointIndex = foundIdx
                            viewModel.log("Start dragging point \(foundIdx)")
                        }
                    }
                    .onEnded { value in
                        if let idx = viewModel.draggedPointIndex {
                            viewModel.commitMove(at: idx, from: value.startLocation)
                            viewModel.draggedPointIndex = nil
                            viewModel.log("Finished dragging point \(idx)")
                        } else {
                            viewModel.addPoint(value.location)
                            viewModel.log("Added point at \(value.location)")
                        }
                    }
            )
            .contextMenu {
                if !viewModel.points.isEmpty {
                    Button("Delete Point") { viewModel.deletePoint(at: viewModel.points.count - 1) }
                }
            }
        }
    }
}

struct VideoPlayer: NSViewRepresentable {
    var player: AVPlayer
    func makeNSView(context: Context) -> AVPlayerViewWrapper {
        let view = AVPlayerViewWrapper()
        view.player = player
        return view
    }
    func updateNSView(_ nsView: AVPlayerViewWrapper, context: Context) {}
}

class AVPlayerViewWrapper: NSView {
    private let playerLayer = AVPlayerLayer()
    var player: AVPlayer? { get { playerLayer.player } set { playerLayer.player = newValue } }
    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        self.layer = playerLayer
        self.wantsLayer = true
        playerLayer.videoGravity = .resizeAspect
    }
    required init?(coder: NSCoder) { fatalError() }
    override func layout() { super.layout(); playerLayer.frame = self.bounds }
}
