# DOCUMENTATION SOURCE: FxTileableEffect | Apple Developer Documentation.pdf

apiManager
An object that obtains the various API objects your plug-in needs for communicating with the host application.
Building an FxPlug plug-in manually
Migrating FxPlug 3 plug-ins to FxPlug 4
Adding parameters to plug-ins
Thread safety in plug-ins
Using this protocol, your plug-in can render select portions of its output, or tiles, to increase efficiency and to operate
outside the hostʼs process. Your plug-in should saves a weak reference to the apiManager object to use in subsequent
calls. The plug-in initializes its properties and instance variables and returns a pointer to self. If any part of initialization
fails in an unrecoverable way, this method returns nil.
init?(apiManager: any PROAPIAccessing)
Initializes the API manager for your plug-in.
Required
protocol PROAPIAccessing
func finishInitialSetup() throws
Finishes the initial setup and is called exactly once on first application of your plug-in to allow it to do anything else
it needs to in order to finish setting itself up.
Required
func pluginInstanceAddedToDocument()
Notifies your plug-in when it becomes part of userʼs document.
func properties(AutoreleasingUnsafeMutablePointer<NSDictionary>?) throws
Tells the host application about what the plug-in needs and supports, such as whether it renders in a gamma-
corrected color space or a linear one.
Required
func addParameters() throws
Tells the host application what parameters your plug-in requires.
Required
func `class`(forCustomParameterID: UInt32) -> AnyClass
Returns the class of the object contained in the custom parameter with the given ID.
func classes(forCustomParameterID: UInt32) -> Set<AnyHashable>
Returns the classes of the objects contained in the custom parameter with the given ID.
Required
func parameterChanged(UInt32, at: CMTime) throws
Executes when the host detects that a parameter has changed.
func sourceTileRect(UnsafeMutablePointer<FxRect>, sourceImageIndex: Int, sourceImages:
[FxImageTile], destinationTileRect: FxRect, destinationImage: FxImageTile, pluginState:
Data?, at: CMTime) throws
Calculate the input rectangle needed for the given image input and the output tile to be rendered.
Required
func destinationImageRect(UnsafeMutablePointer<FxRect>, sourceImages: [FxImageTile],
destinationImage: FxImageTile, pluginState: Data?, at: CMTime) throws
Calculates the bounds of the output image determined by the various inputs and plug-in state at the given render
time.
Required
func FxRectsAreEqual(FxRect, FxRect) -> Bool
Compares two rectangle structures and determines if they are equal.
func pluginState(AutoreleasingUnsafeMutablePointer<NSData>?, at: CMTime, quality: Fx
Quality) throws
Retrieves the plug-inʼs parameter values, performs any calculations it needs to from those values, and packages up
the result to be used later with rendering.
Required
func scheduleInputs(AutoreleasingUnsafeMutablePointer<NSArray?>?, withPluginState: Data
?, at: CMTime) throws
Tells the host application how many frames from the given input sources your plug-in requires in order to render at
the given render time.
func renderDestinationImage(FxImageTile, sourceImages: [FxImageTile], pluginState: Data
?, at: CMTime) throws
Sends a request that the host wants your plug-in to do its rendering for a given output image tile.
Required
Data types defined in the FxPlug SDK that may not be class-specific.
typealias FxDepth
Constants used to identify bit depth.
typealias FxField
Constants used to identify a field.
typealias FxFieldOrder
Constants used to identify the field order of an image stream.
typealias FxPoint2D
A 2D point representation.
struct FxPoint3D
A 3D point representation
typealias FxQuality
Constants used to identify rendering quality.
struct FxRect
A 2D axis-aligned rectangle with integer coordinates.
typealias FxSize
A structure to store width and height values.
typealias FxDrawingCoordinates
Constants used to identify coordinate spaces.
typealias FxError
Errors returned by plug-in hosts applications.
typealias FxImageColorInfo
Identifies some color properties of an FxImage instance. These include the color space, gamma level, and, in the
case of YCbCr images, the color matrix for conversion to RGB.
var kFxRect_Empty: FxRect
An empty rectangle.
var kFxRect_Infinite: FxRect
An infinite rectangle.
typealias FxQuality
Constants used to identify rendering quality.
typealias FxDepth
Constants used to identify bit depth.
let FxPlugErrorDomain: String
The error domain for FxPlug-related errors.
NSObjectProtocol
Rendering in FxPlug
Use Metal or other frameworks to render images with your FxPlug plug-in.
Communicating with the plug-in state
Prepare the necessary information, such as parameter values, for your FxPlug plug-in to render.
Working with tiled images
Render only the necessary tiles of an image in your FxPlug plug-in to improve efficiency.
Optimizing FxPlug plug-ins
Maintain consistent rendering at all resolutions and aspect ratios by using pixel transforms.
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
var kFxPropertyKey_DesiredProcessingColorInfo: StringV
var kFxPropertyKey_NeedsFullBuffer: StringV
var kFxPropertyKey_VariesWhenParamsAreStatic: StringV
/Filter
Parameters
Mentioned in
Overview
To p i c s
Setting up your plug-in
Adding and setting parameters
Determining input and output bounds
Rendering an output
Data types
Constants
Relationships
Inherits From
See Also
Rendering

Developer Documentation
Professional Video Applications/ FxTileableEffect
Protocol
FxTileableEffect
The designated initializer for your plug-in for rendering only certain portions of the plug-inʼs output,
referred to as tiles.
FxPlug 4.0+
protocol FxTileableEffect : NSObjectProtocol
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


