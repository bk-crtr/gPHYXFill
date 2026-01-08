# DOCUMENTATION SOURCE: FCPXML Reference | Apple Developer Documentation.pdf

Use FCPXML to describe the media, metadata, and other items users send and receive between your app and Final Cut
Pro.
FCPXML uses a basic XML document structure and specially defined elements, to describe the media assets, projects,
and metadata users send to Final Cut Pro, and the rendered media, timeline sequences, editing decisions, and other
data your app receives from Final Cut Pro.
This reference guide provides information about using FCPXML, including:
Document structure
Elements
Resources
Note
FCPXML 1.9 requires Final Cut Pro 10.4.9 or later. It describes certain, but not all, aspects of projects and events
useful for other applications. But, itʼs not a substitute for a native Final Cut Pro project and event data organized in a
library bundle.
The following resources are helpful as you work with the FCPXML format:
Final Cut Pro User Guide
Final Cut Pro Resources
Extensible Markup Language (XML) 1.0 specification
Creating FCPXML Documents
Describe media assets, editing decisions, metadata, and other items in FCPXML so users can send and receive
data between your app and Final Cut Pro.
FCPXML Bundle Reference
Keep an FCPXML document and any files it references together in a bundle.
Importing FCPXML Data
Import data from Final Cut Pro to your app by using FCPXML.
fcpxml
Contain top-level elements in a document.
resources
Contain shared resources on which events and projects depend.
library
Contain events, clips, projects, and smart collections.
event
Represent a single event in a library.
project
Represent a project timeline.
import-options
Contain options that describe how to import events and projects into Final Cut Pro.
Story Elements
Describe Final Cut Pro events and project data and their associated effects.
asset
Define file-based media managed in a Final Cut Pro library.
media
Describe a compound clip or a multi-camera media definition.
format
Reference a video-format definition.
effect
Reference visual, audio, or custom effects.
locator
Describe a URL-based resource.
object-tracker
Describe a tracked object such as a face or other moving object in a video clip.
tracking-shape
Define a shape that the object-tracker uses to match the movement of an object.
Describing Final Cut Pro Items in FCPXML
Describe clips, projects, and other items in FCPXML to exchange data with Final Cut Pro.
keyword-collection
Group clips and projects based on matching keywords.
smart-collection
Describe smart collection filters that group clips and projects that match the criteria.
collection-folder
Contain keyword and smart collections.
Associating Ratings, Keywords, Markers, and Metadata with Media
Organize and annotate the media and projects your users send to and receive from Final Cut Pro.
Metadata
Describe metadata about media that is of interest to other applications.
Metadata Keys and Sources
Define metadata keys that identify each metadata item and its sources.
Document Type Definition
Document Type Definition (DTD) for the latest Final Cut Pro XML interchange format.
Content and Metadata Exchanges with Final Cut Pro
Send media assets and timeline sequences to Final Cut Pro for editing, and receive rendered media and editing
decisions in your app.
Workflow Extensions
Integrate your appʼs workflow within the Final Cut Pro interface to streamline data exchange.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
To p i c s
Document Structure and Root Elements
Media Elements and Effects
Shared Resources and Reference Elements
Clips and Project Collections
Media Metadata
Document Type Definition
See Also
XML Data Exchange

Developer Documentation
Professional Video Applications/ FCPXML Reference
FCPXML Reference
Create documents that describe the data your app or workflow extension exchanges with Final Cut
Pro.
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


