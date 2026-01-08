# DOCUMENTATION SOURCE: Encoder Extensions | Apple Developer Documentation.pdf

The Compressor app provides a variety of transcoding formats to convert Final Cut Pro source media files into most
common media formats. Create encoder extensions to add custom transcoding formats to the Compressor app,
allowing Final Cut Pro users to convert their source media files to these output formats.
You create an encoder extension using the Xcode template that comes with the SDK. The SDK has APIs that let you
exchange data with the Compressor app and take advantage of Qmasterʼs distributed processing service to improve the
transcoding time in your extension.
You distribute the encoder extension inside a macOS app. When a user installs the macOS app containing your
extension, the extensionʼs custom file format becomes available in the Compressor appʼs Settings interface. Users can
then choose your custom transcoding format to encode their Final Cut Pro projects. After encoding the source file into
the custom format, the extension writes the encoded output to a user-specified destination.
Important
The Compressor SDK is only available for encoder extensions written in Objective-C.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview

Developer Documentation
Professional Video Applications/ Encoder Extensions
API Collection
Encoder Extensions
Add custom output file formats to the Final Cut Pro workflow.
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


