# DOCUMENTATION SOURCE: FxPlug | Apple Developer Documentation.pdf

When the effect or look you want to achieve canʼt be created with the filters or generators provided with Final Cut Pro or
Motion, use the FxPlug software development kit (SDK) to write your own custom visual effects. FxPlug is a compact,
powerful image-processing plug-in architecture that lets you create unique, hardware-accelerated or CPU-based
effects plug-ins with customized UI and onscreen controls.
FxPlug consists primarily of Objective-C protocol definitions. You create a plug-in by writing code in Objective-C,
Objective-C++, or Swift that conforms to these protocols, implementing the methods declared by the protocols. The
host application provides the capabilities in all the protocols that have the API suffix. Your plug-in is responsible for
implementing the other protocols.
FxPlug 4 introduces fully “out-of-process” FxPlug plug-ins, which have no component that runs inside of the host
application process. Out-of-process plug-ins provide improved security for end users and allow plug-in developers the
freedom to choose from a variety of rendering technologies, such as OpenGL, Core Graphics, Core Image, or Metal to
develop unique plug-ins that include on-screen controls and custom user interface elements—all running seamlessly in
the host application. Plug-in developers can choose to implement in either Swift or Objective-C.
Note
OpenGL and OpenCL are deprecated in macOS.
Additionally, the new FxTileableEffect API lets third-party plug-ins render only portions of the output (known as
tiles) for more efficiency, in the same manner as Appleʼs own plug-ins.
This documentation highlights new and updated concepts and APIs in FxPlug 4. You can find documentation for legacy
plug-ins created with FxPlug 3 in the archived FxPlug 3.1.1 documentation.
Note
FxPlug 3 isnʼt supported in Final Cut Pro or Motion.
Setting up for FxPlug development
Download and install the required software to develop FxPlug plug-ins.
Using out-of-process FxPlug plug-ins
Register and render FxPlug plug-ins with Motion and Final Cut Pro.
Building an FxPlug plug-in from an Xcode template
Create a plug-in in Xcode with the FxPlug template.
Building an FxPlug plug-in manually
Create your own plug-in in Xcode.
Using FxPlug APIs
Use various FxPlug APIs to communicate with host apps like Motion or Final Cut Pro.
Editing property lists for FxPlug plug-ins
Modify the way hosts recognize and display your FxPlug plug-in.
Thread safety in plug-ins
Learn about reentrancy for plug-ins and best practices to make them thread safe.
Rendering in FxPlug
Use Metal or other frameworks to render images with your FxPlug plug-in.
Communicating with the plug-in state
Prepare the necessary information, such as parameter values, for your FxPlug plug-in to render.
Working with tiled images
Render only the necessary tiles of an image in your FxPlug plug-in to improve efficiency.
Optimizing FxPlug plug-ins
Maintain consistent rendering at all resolutions and aspect ratios by using pixel transforms.
protocol FxTileableEffect
The designated initializer for your plug-in for rendering only certain portions of the plug-inʼs output, referred to as
tiles.
Adding parameters to plug-ins
Create standard and custom user-facing parameters for your plug-in that will appear in the inspector.
Adding onscreen controls to plug-ins
Simplify user interaction by using onscreen controls for your FxPlug plug-in.
protocol FxPathAPI_v3
An API that defines the methods to retrieve information about paths, shapes, and masks the user has drawn on an
object.
protocol FxUndoAPI
An API that defines the methods that the host app implements to handle plug-in management of the host appʼs
undo queue.
protocol FxCommandAPI
Commands that you can tell the host to perform.
protocol FxCommandAPI_v2
Adds functionality to move the playhead to a specific timeline time.
protocol FxRemoteWindowAPI
A protocol that allows the plug-in to request that the host create a window.
protocol FxRemoteWindowAPI_v2
A protocol that allows the plug-in to request that the host create a window with a defined minimum and maximum
size.
protocol Fx3DAPI_v5
An API that defines the methods the host application provides to get information about the 3D environment,
including camera and object transforms.
protocol FxLightingAPI_v3
An API you use to get information about lights in a scene in a Motion project.
class FxMatrix44
The FxMatrix class encapsulates a 4x4 matrix object and provides matrix inversion and transforming of 2D and
3D points.
Managing color space and gamut in plug-ins
Control the appearance of your rendering by using the color gamut API.
protocol FxColorGamutAPI_v2
A protocol that handles plug-in queries to the host for the projectʼs color gamut.
Understanding time in FxPlug
Learn about time handling in host apps and plug-ins.
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
protocol FxProjectAPI
Methods you use to get information about the project in which your plug-in instance is running.
protocol FxProjectAPI_v2
The method you use to get aspect ratio information about the project in which your plug-in instance is running.
Testing FxPlug plug-ins
Test and debug FxPlug plug-ins using a variety of methods.
Preparing plug-ins for use in Final Cut Pro
Add a plug-in to a Motion effect template to use in Final Cut Pro.
Notarizing your FxPlug plug-in
Give users confidence in your FxPlug plug-in by enabling notarization.
Migrating FxPlug 3 plug-ins to FxPlug 4
Update existing FxPlug 3 plug-ins to out-of-process FxPlug 4 plug-ins.
Versioning and obsoleting old plug-ins
Version plug-ins to add new capabilities, or obsolete them to remove them from the Motion Library.
protocol FxVersioningAPI
A collection of methods that a host application implements to identify the plug-in version used by a project when it
was first created.
FxPlug macros
Create an effect template for use in Final Cut Pro
Use Motion to create custom filters, generators, and transitions for Final Cut Pro.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
To p i c s
Essentials
Plug-in fundamentals
Rendering
User interface
3D and lighting
Color
Time and analysis
Accessing host data
Te s t i n g  a n d  d e p l o y m e n t
Legacy plug-ins
Macros
See Also
Effects

Developer Documentation
Professional Video Applications/ FxPlug
API Collection
FxPlug
Create custom effects plug-ins for Final Cut Pro and Motion.
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
Documentation Language: Swift
News
Discover
Design
Develop
Distribute
Support
Account


