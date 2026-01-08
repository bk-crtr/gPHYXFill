# DOCUMENTATION SOURCE: Building an FxPlug plug-in manually | Apple Developer Documentation.pdf

Every FxPlug plug-in is contained within an application bundle, and uses an XPC service to communicate with the host
applications, Final Cut Pro and Motion. In most cases, you can use the Xcode template thatʼs installed with the FxPlug
SDK as a base your FxPlug plug-in (see Building an FxPlug plug-in from an Xcode template). However, if youʼre creating
an especially complex plug-in (or if youʼre the type who likes to know behind-the- scenes details), then this article is for
you.
You can create your own FxPlug plug-in thatʼs discoverable by Motion and ready to be developed for your specific
rendering goals. Begin by creating your plug-inʼs structure, making the plug-in discoverable, and building and running
your project to check for errors. Next, create your plug-in class, cache the host API manager, and define a properties
dictionary. You then implement required methods to specify how the plug-in will render and to establish the plug-in
state.
Note
This document describes how to create a plug-in with FxPlug 4. FxPlug 3 isnʼt supported in Final Cut Pro and
Motion. For more information, see Migrating FxPlug 3 plug-ins to FxPlug 4.
To build the structure for your plug-in, follow these steps:
T. Create an empty app. Launch Xcode and create a new project. Select macOS to display the appropriate macOS
templates. Then, select the App template, and click Next. Enter  FxPlugBrightness as the Product Name for your
app. For this example, choose Objective-C or Swift as the language, and select XIB for User Interface. Deselect
Include Tests. Click Next. Specify a location for your project, and then click Create.
Note
The plug-in application bundle can also present a UI made with Storyboards or SwiftUI.
Y. In the Xcode Editor toolbar, click the “Build and run” button. Once the build successfully completes, a typical
macOS application launches and displays an empty default window. (If you select the window, you see the name of
your app, FxPlugBrightness, in the menu bar). Quit the app.
[. Add an XPC service inside your new app. In Xcode, click the “Show projects and targets list” button to toggle a list
of your appʼs project and build targets in the sidebar. In the Targets pane, select your app (if itʼs not already selected).
Click the plus sign (+) at the bottom of the pane, select XPC Service from the Framework & Library section, and click
Next. Give your XPC service a name, such as “FxBrightnessXPC” . This is the name used to report any problems your
host application has in the discovery process, so be sure to make note of it. Make sure the name of your app is
displayed in the Embed in Application option, and then click Finish.
a. Add the FxPlug and PluginManager frameworks. In the Xcode Project and Targets sidebar pane, select the XPC
service target you just added. Select General from the project menu bar, locate the Frameworks and Libraries section,
and click the plus sign at the bottom of that section. Select FxPlug.framework from the list of frameworks and
click add. Repeat for the PluginManager.framework. You may need to click “Add Other,” and locate the
frameworks, which are installed in /Library/Developer/Frameworks.
c. Add a Copy Files phase. Choose Build Phases from the Project menu bar and click the plus sign in the upper-left
corner of that pane. Choose New Copy Files Phase from the menu and then expand the new section. Click the plus
sign at the bottom of the new Copy Files section, select FxPlug.framework, and click Add. Repeat for the Plugin
Manager.framework. See Code sign embedded frameworks for information on scripting the copy and code sign
phase for PluginManager.framework.
d. Add a framework search path. In the Targets pane, select your XPC service target and choose Build Settings from
the project menu bar. Select All to make sure youʼre seeing all available settings. In the Build Settings pane, search for
Framework Search Paths. Expand the Framework Search Paths item, click the plus sign, and enter the path to where
the FxPlug SDK is installed: /Library/Frameworks $(inherited). Do this for both debug and release.
e. Add Additional SDKs path. With your XPC service target still selected in the Targets pane, search for “Additional
SDKs” and add the path /Library/Developer/SDKs/FxPlug.sdk as shown below. Do this for both debug and
release.
macOS uses a technology called PlugInKit to discover and register plug-ins on your system. When a host application
launches, PluginKit reports a list of plug-ins that are available for that host. To make your plug-in discoverable by
PluginKit, make pluginkit your Wrapper Extension. In the Targets list, select your XPC service, select Build Settings
from the Project menu bar, and select All to make sure youʼre seeing all available settings. Itʼs helpful to search for
“packaging” to find the correct section.
Next, enter pluginkit in place of the default Wrapper Extension.
Tell Xcode where to embed the plug-in bundle in your application bundle:
T. Select your wrapper app from the Targets list.
Y. From the Project menu bar, select Build Phases. If you donʼt already have a Copy Files phase item, first add one by
clicking the Add A New Build Phase (+) button and then selecting New Copy Files Phase.
[. Find and expand the Copy Files phase item and then click the Add Items (+) button and at the Destination option,
choose your XPC service. For example, the FxBrightnessXPC service would look like this, embedded.
a. Configure your plug-in to be recognized as an FxPlug plug-in by modifying the Info.plist file for your XPC
service. For a complete list of configuration requirements, see Editing property lists for FxPlug plug-ins.
The host application must make an XPC connection to your plug-in. To do that, your plug-inʼs XPC must call the Fx
Principal starting method in its main.swift file. Be sure to import the FxPlug/FxPlugSDK.h header file. In
Objective-C, import it in your main.m file. In Swift, import it in your XPC Service-Bridging-Header.h file.
When using Swift, also add the following code to your main.swift file:
Once you finish editing your Info.plist, build and run your project. It should compile without errors and launch
without complications. When you launch Motion, you see your plug-in either in the filter list or in the generator list
(depending on whether your entry in protocolNames was FxFilter or FxGenerator as you indicated when Editing
property lists for FxPlug plug-ins).
Confirm that your plug-in was discovered by querying PlugInKit. In Terminal, type the following:
Terminal displays a list of every FxPlug plug-in on your system. If this list is very long, you can search the Terminal
output for the specific name of your plug-in, like this:
Terminal now displays FxPlugBrightness(1.0). If you want more information about your discovered plug-in, add
the verbose option like this:
Terminal now displays the following:
Note
Even though your plug-in has been discovered, itʼs still not ready to use. If you use the plug-in in the library, you get
unexpected results.
When a host application instantiates your plug-in, it looks for the className item in the Info.plist file for your XPC
service. This class is where you write your plug-in code — making sure that it conforms to the FxTileableEffect
protocol. (See Rendering in FxPlug for more information).
T. Select File > New > File and add a class to your project.
Y. Set the name of the class to the className you entered in your Info.plist file. For example, call it FxPlug
BrightnessFilter.
[. Set the Target Membership of the new class to the XPC service.
a. Open the source file that defines your class. In Objective-C, open the header file. In Swift, open the .swift source
file.
c. In Objective-C, import FxPlugSDK.h at the top of your header file.
d. Make your class conform to the FxTileableEffect protocol, as shown in the following example.
Before you can add methods for the FxTileableEffect protocol, use the following code to cache the host API
manager. For more on the role of the host API, see Using FxPlug APIs.
Declare the API Manager and initializer in your source file. For example, add this code to the FxPlugBrightness
Filter.swift file in Swift, or to the FxPlugBrightnessFilter header file in Objective-C.
In Objective-C, add the code for the initializer to the FxPlugBrightnessFilter implementation .m file:
Every plug-in defines a properties dictionary that holds configuration values unique to how that plug-in operates,
including keys that specify if the plug-in can change its output size and if it supports tiled rendering. To define a
properties dictionary for your plug-in, implement this method:
For information on other available key values, see FxBaseEffect and FxTileableEffect. There are many options,
such as:
kFxPropertyKey_NeedsFullBuffer
Indicates if this plug-in needs the entire image to do its processing, and canʼt tile its rendering.
kFxPropertyKey_MayRemapTime
Indicates that your plug-in samples input from times other than the current time.
kFxPropertyKey_VariesWhenParamsAreStatic
Indicates whether this effect changes its rendering even when the parameters donʼt change.
Note
The property keys kFxPropertyKey_IsThreadSafe and kFxPropertyKey_UsesRationalTime are ignored
for FxPlug 4 and newer plug-ins because all FxPlug 4 plug-ins are required to be thread-safe and use CMTime.
Plug-ins generally require user interface elements such as parameter sliders, image wells, and color pickers. Add the
addParameters() method to your plug-in. For more information about parameter types, and examples of their usage,
see Using FxPlug APIs and Adding parameters to plug-ins.
You may choose to have your plug-in handle any changes in parameters made by your users (and relayed through the
plug-inʼs UI elements) with the parameterChanged(_:at:) method. Implement this optional method in your plug-in:
The remaining methods required for your plug-in specify how your plug-in renders its effects:
You will also need to implement two methods to set your source and destination rectangles:
Finally, your plug-in must prepare a “plug-in state” object when requested by the host app. To do this, implement the
following method in your plug-in:
For more information on rendering, FxImageTile, or source and destination rectangles, see Rendering in FxPlug and
Working with tiled images. For more information on plug-in state, see Communicating with the plug-in state. For more
information on timing in FxPlug, see Scheduling media in plug-ins.
Building an FxPlug plug-in from an Xcode template
Create a plug-in in Xcode with the FxPlug template.
Using FxPlug APIs
Use various FxPlug APIs to communicate with host apps like Motion or Final Cut Pro.
Editing property lists for FxPlug plug-ins
Modify the way hosts recognize and display your FxPlug plug-in.
Thread safety in plug-ins
Learn about reentrancy for plug-ins and best practices to make them thread safe.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
Build the structure for your plug-in
Chooseoptionsforyournewproject:ProductName:FxPlugBrightnessTe a m :Addaccount...OrganizationIdentifier:com.myCompanyBundleIdentifier:com.myCompany.FxPlugBrightnessLanguage:SwiftCancelPreviousNext
Make your plug-in discoverable
Connect the host application to the plug-in
Objective-C
#import <FxPlug/FxPlugSDK.h>
FxPrincipal.startServicePrincipal()
Build and run your project
pluginkit -m -p FxPlug
pluginkit -m -p FxPlug | grep “FxPlugBrightness”
pluginkit -mv -p FxPlug | grep “FxPlugBrightness”
FxPlugBrightness(1.0)DE175335-4588-4C40-84D6-BCD4F0BCAC44 2019-07-09 15:41:18 +0000 /Users/your_username/Desktop/FxPlugBrightness/Build/Products/Debug/ FxPlugBrightness.app/Contents/PlugIns/FxPlugBrightness.pluginkit
Create your plug-in class
Objective-C
@objc(FxBrightnessFilter) class FxBrightnessFilter : NSObject, FxTileableEffect {
Cache the host API manager
Objective-C
    let _apiManager : PROAPIAccessing!
    
    required init?(apiManager: PROAPIAccessing) {
        _apiManager = apiManager
    }
- (nullable instancetype)initWithAPIManager:
(id<PROAPIAccessing>)newApiManager;
{ 
    self=[super init];
    if (self !=nil)
    { 
        _apiManager = newApiManager;
    } 
    return self;
}
Define a properties dictionary
Objective-C
func properties(_ properties: AutoreleasingUnsafeMutablePointer<NSDictionary>?) throws {
        let swiftProps = [
            kFxPropertyKey_MayRemapTime : NSNumber(booleanLiteral: false),
            kFxPropertyKey_PixelTransformSupport : NSNumber(value: kFxPixelTransform_Full),
            kFxPropertyKey_VariesWhenParamsAreStatic: NSNumber(booleanLiteral: false),
            kFxPropertyKey_ChangesOutputSize : NSNumber(booleanLiteral: false)
        ]
        let props = NSDictionary(dictionary: swiftProps)
        properties?.pointee = props
    }
Implement required and optional methods
Objective-C
func addParameters()
Objective-C
func parameterChanged(_ paramID: UInt32, at time: CMTime)
Objective-C
func renderDestinationImage(_ destinationImage: FxImageTile, sourceImages: [FxImageTile], pluginState: 
Objective-C
func sourceTileRect(_ sourceTileRect: UnsafeMutablePointer<FxRect>, sourceImageIndex: UInt, sourceImages
func destinationImageRect(_ destinationImageRect: UnsafeMutablePointer<FxRect>, sourceImages: [FxImageTile
Objective-C
func pluginState(_ pluginState: AutoreleasingUnsafeMutablePointer<NSData>?, at renderTime: CMTime, quality
See Also
Plug-in fundamentals

Developer Documentation
Professional Video Applications/ FxPlug / Building an FxPlug plug-in manually
Article
Building an FxPlug plug-in manually
Create your own plug-in in Xcode.
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
Swift
Swift
Swift
Swift
Swift
Swift
Swift
Swift
Documentation Language: Swift
News
Discover
Design
Develop
Distribute
Support
Account


