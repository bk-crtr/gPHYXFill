# DOCUMENTATION SOURCE: Workflow Extensions | Apple Developer Documentation.pdf

When your app supports workflows that require users to exchange data between your app and Final Cut Pro, consider
adding a workflow extension to your app. A workflow extension is not a replacement for your app; itʼs an extension of the
appʼs functionality, made available within the Final Cut Pro interface. When a userʼs workflow requires frequent switching
between your app and the Final Cut Pro interface, having your appʼs functionality within the interface provides a
seamless user experience.
You must use a macOS app to contain and deliver your workflow extension. The app that contains the extension is called
the container app.
When a user launches an extension, Final Cut Pro hosts the extension in its interface, inside a floating window, and
becomes the host app. However, the amount of information a workflow extension presents in a floating window space
may not be sufficient for complex tasks. Create an extension for workflows that are optimized for limited screen size; for
example, managing media files, and browsing and accessing stock footage. Delegate time-consuming or complicated
tasks to the container app.
You can extend a workflow extensionʼs functionality beyond data exchange by using the libraries provided in the
Workflow Extension SDK. The libraries facilitate a workflow extensionʼs interaction and communication with the Final Cut
Pro timeline. You can use the APIs in the SDK to support workflows that allow users to collaborate in real time on a
project opened in both Final Cut Pro and the workflow extension.
Designing Workflow Extensions
Follow these design guidelines to provide a unique experience to workflow extension users.
Building a Workflow Extension
Create a workflow extension in Xcode by using the Final Cut Pro Workflow Extension template.
ProExtensionPrincipalViewControllerClass
The name of the principal view controller class of your extension.
ProExtensionAttributes
A dictionary that specifies the minimum size of the floating window in which Final Cut Pro hosts the extension view.
Interacting with the Final Cut Pro Timeline
Extend your workflows beyond media exchange by enabling a workflow extension to interact with the Final Cut Pro
timeline.
protocol FCPXHost
A protocol that provides an interface to retrieve the Final Cut Pro timeline proxy objects and details of the host app.
func ProExtensionHostSingleton() -> (any NSObjectProtocol)?
Returns the singleton proxy instance of the host object.
class FCPXTimeline
An interface that has methods and properties to communicate and interact with the Final Cut Pro timeline.
protocol FCPXTimelineObserver
An interface with optional methods implemented by observers of FCPXTimeline objects.
class FCPXObject
An abstract superclass for Final Cut Pro timeline proxy objects.
enum FCPXObjectType
The Final Cut Pro timeline object types.
class FCPXLibrary
An object that contains details of a Final Cut Pro library.
class FCPXEvent
An object that contains details of an event in the Final Cut Pro library.
class FCPXProject
An object that contains details of a project with the sequence open in the Final Cut Pro timeline.
class FCPXSequence
An object that contains details of a sequence thatʼs open in the Final Cut Pro timeline.
enum FCPXSequenceTimecodeFormat
The display format of the sequence timecode.
Content and Metadata Exchanges with Final Cut Pro
Send media assets and timeline sequences to Final Cut Pro for editing, and receive rendered media and editing
decisions in your app.
FCPXML Reference
Create documents that describe the data your app or workflow extension exchanges with Final Cut Pro.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
To p i c s
Essentials
Information Property List Keys
FCPX Interactions
FCPX Timeline Proxy Objects
See Also
XML Data Exchange

Developer Documentation
Professional Video Applications/ Workflow Extensions
API Collection
Workflow Extensions
Integrate your appʼs workflow within the Final Cut Pro interface to streamline data exchange.
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


