import SwiftUI

@main
struct gPHYXEditorApp: App {
    @State private var videoPath: String = ""
    @State private var instanceID: String = ""

    init() {
        let args = ProcessInfo.processInfo.arguments
        if let videoIdx = args.firstIndex(of: "--video"), videoIdx + 1 < args.count {
            _videoPath = State(initialValue: args[videoIdx + 1])
        }
        if let iidIdx = args.firstIndex(of: "--instance"), iidIdx + 1 < args.count {
            _instanceID = State(initialValue: args[iidIdx + 1])
        }
    }

    var body: some Scene {
        WindowGroup {
            ContentView(videoPath: videoPath, instanceID: instanceID)
        }
    }
}
