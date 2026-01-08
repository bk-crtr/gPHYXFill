# DOCUMENTATION SOURCE: Understanding time in FxPlug | Apple Developer Documentation.pdf

FxPlug 4 uses rational time of CMTime type to handle all timing in the host apps and plug-ins. Time can be in the form of
a specific time or duration in an effect, object, or timeline (project). When FxPlug methods use time or renderTime,
that time generally refers to the current time that the host is providing or requesting. FxTimingAPI_v4 provides
methods to access other time values available in the host app. For FxPlug methods that donʼt provide time, you can use
FxCustomParameterActionAPI_v4ʼs currentTime() to determine current playhead time in the project.
In Motion projects, timelines always start at zero, regardless of the custom timecode the user may set in the projectʼs
properties. Input time is also always relative to the timeline start time. Other times of significance are indicated in the
following figure.
In Final Cut Pro, projects can start at any timecode set by the user, and their timeline time will be continuous from that
start time. Timeline time is independent of Input Time in Final Cut Pro, so FxTimingAPI_v4 provides two convenience
methods to convert between them: timelineTime(_:fromInputTime:) and inputTime(_:fromTimeline
Time:). The following figure shows various times in the FxTiming API.
Note
Although the timelineʼs start time is set by the user, the timecode of the timeline will not necessarily correspond to
the rational time your plug-in uses. This difference may be due to drop-frame timecode, or because the timecode
resets to 00:00:00:00. For example, if the user sets the project start timecode to 23:59:50:00, the rational
start time of the timeline when converted to seconds may be 86476.39 seconds, but will NOT reset to 0 seconds
when the timelineʼs timecode reaches 00:00:00:00.
You must apply FxPlug plug-ins to a Motion template to use it in a Final Cut Pro project. You may notice significant
differences in how startTime(forEffect:), durationTime(forEffect:), and startTimeOfInput(to
Filter:) return time values. To understand these differences, imagine that an effect template added to a clip in Final
Cut Pro is equivalent to a complete timeline in Motion. For example, in Motion, startTime(forEffect:) will return
the plug-in effectʼs start time relative to the Motion timeline. In Final Cut Pro, the same method will return the start time
of the plug-in effect, relative to the native start time of the clip to which the user applies the template, regardless of the
how much of the clipʼs head has been trimmed by the user.
Effect templates that you apply to clips or other objects in Final Cut Pro may stretch to match the duration of the clip.
When clips with effect templates are retimed by the user in Final Cut Pro, the effect template will adjust proportionally.
Unlike in Motion, frameDuration(_:) and sampleDuration(_:) always return the native frame or sample
durations of the clip, regardless of how fast or slow you retime it. If your plug-in attempts to determine the frame
duration or frames per second of the timeline, then use timelineFpsNumerator(for:) and timelineFps
Denominator(for:) to make that calculation, as shown in the example below.
Scheduling media in plug-ins
Use the scheduling APIs in FxPlug to retrieve frames from different times.
protocol FxTimingAPI_v4
A protocol that defines the methods provided by the host, so that a plug-in can query the timing properties of its
input.
protocol FxKeyframeAPI_v3
A collection of methods for manipulating the keyframes of your FxPlug 4 plug-in.
Analyzing media
Use the FxPlug analysis API to analyze frames of source media before rendering them.
protocol FxAnalysisAPI
A protocol that host applications implement to provide information to plug-ins that support the FxAnalyzer
protocol.
protocol FxAnalysisAPI_v2
A protocol that host applications implement to provide information to plug-ins that support the FxAnalyzer
protocol.
protocol FxAnalyzer
A protocol you implement in your plug-in to analyze frames that the plug-in is applied to.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
Time in Motion
Time in Final Cut Pro
Motion effect templates
Retiming and frame duration
Objective-C
let timingAPI = _apiManager!.api(for: FxTimingAPI_v4.self) as! FxTimingAPI_v4
var timelineFrameDuration = CMTimeMake(value: Int64(timingAPI.timelineFpsDenominator(for: self)),
                                           timescale: Int32(timingAPI.timelineFpsNumerator(for: self)))
See Also
Time and analysis

Developer Documentation
Professional Video Applications/ FxPlug / Understanding time in FxPlug
Article
Understanding time in FxPlug
Learn about time handling in host apps and plug-ins.
Platforms
iOS
iPadOS
macOS
tvOS
visionOS
watchOS
Tools
Swift
SwiftUI
Swift Playground
TestFlight
Xcode
Xcode Cloud
SF Symbols
Topics & Technologies
Accessibility
Accessories
App Extension
App Store
Audio & Video
Augmented Reality
Design
Distribution
Education
Fonts
Games
Health & Fitness
In-App Purchase
Localization
Maps & Location
Machine Learning & AI
Open Source
Security
Safari & Web
Resources
Documentation
Tutorials
Downloads
Forums
Videos
Support
Support Articles
Contact Us
Bug Reporting
System Status
Account
Apple Developer
App Store Connect
Certificates, IDs, & Profiles
Feedback Assistant
Programs
Apple Developer Program
Apple Developer Enterprise Program
App Store Small Business Program
MFi Program
News Partner Program
Video Partner Program
Security Bounty Program
Security Research Device Program
Events
Meet with Apple
Apple Developer Centers
App Store Awards
Apple Design Awards
Apple Developer Academies
WWDC
Swift
Documentation Language: Swift
News
Discover
Design
Develop
Distribute
Support
Account


