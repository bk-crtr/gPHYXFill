# DOCUMENTATION SOURCE: kFxPropertyKey_DesiredProcessingColorInfo | Apple Developer Documentation.pdf

Managing color space and gamut in plug-ins
The value for this key is an NSUInteger indicating the color space that the plug-in will process in. This color space is
expressed as an FxImageColorInfo constant. If a plug-in includes this property, all inputs are in the specified color
space, and the output must also be in this color space.
var kFxPropertyKey_NeedsFullBuffer: String
A key that determines whether the plug-in needs the entire image to do its processing, and canʼt tile its rendering.
var kFxPropertyKey_VariesWhenParamsAreStatic: String
A key that determines whether your rendering varies even when the parameters remain the same.
var kFxPropertyKey_ChangesOutputSize: String
A key that determines whether your filter has the ability to change the size of its output to be different than the size
of its input.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
Effects
FxPlug
Create an effect template for use in Final Cut Pro
XML Data Exchange
Content and Metadata Exchanges with Final Cut Pro
Workflow Extensions
FCPXML Reference
Compressor Encoder Extensions
Encoder Extensions
Reference
Professional Video Applications Enumerations
Professional Video Applications Constants
Professional Video Applications Data Types
Professional Video Applications Protocols
Variables
var kFxPropertyKey_ChangesOutputSize: StringV
var kFxPropertyKey_DesiredProcessingColorInfo: St…V
var kFxPropertyKey_NeedsFullBuffer: StringV
var kFxPropertyKey_VariesWhenParamsAreStatic: StringV
/Filter
Mentioned in
Discussion
See Also
Property Keys

Developer Documentation
Professional Video Applications/ kFxPropertyKey_DesiredProcessingColorInfo
Global Variable
kFxPropertyKey_DesiredProcessingColorInfo
A key that determines whether your plug-in renders in linear or gamma-corrected color space.
FxPlug 3.1+
var kFxPropertyKey_DesiredProcessingColorInfo: String { get }
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


