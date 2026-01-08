# DOCUMENTATION SOURCE: Editing property lists for FxPlug plug-ins | Apple Developer Documentation.pdf

FxPlug plug-ins typically have more than one Info.plist file: one in the application bundle and one in the XPC
service. Although the applicationʼs Info.plist doesnʼt require any changes, your XPCʼs Info.plist requires some
changes to create a functioning plug-in.
Add the following key-value pairs to your XPC Info.plist, using the associated values listed below for your plug-in.
Below is an example of a complete Info.plist.
Tip
Add entries to property lists by control-clicking in the blank space below the last entry and selecting Add Row, or
hovering the mouse over an existing entry and clicking the + button. After selecting the appropriate key (or typing it
in manually) change the type for the type of key you require (String, Number, Dictionary, Array, etc.) To add
components to a Dictionary or Array, first turn down the arrow before clicking the + button.
This dictionary contains three items: A PrincipalClass key, a Protocol key, and an Attributes dictionary with
the component keys com.apple.protocol with the string value FxPlug, and com.apple.version with a version
number. Set the Protocol key to PROXPCProtocol, as this is what is used to identify your plug-in as an FxPlug to
PlugInKit. Set the PrincipalClass string here to FxPrincipal.
By adding these keys, you make your XPC discoverable by PlugInKit. Itʼs important to update your version number when
you make updates—if the host app finds more than one plug-in with the same name, it returns the plug-in with the
newest version first.
Both of these keys are arrays of values that tell the host application how to name and categorize your effect.
Hereʼs a short description of what each one does:
ProPlugPlugInGroupList lists the groupName and the groupʼs UUID to which your plug-in should be added. You
can provide a new groupName and group UUID, or use an existing one. If multiple plug-ins list the same group UUID,
theyʼre joined together in that group when displayed in Motionʼs Library.
In the ProPlugPlugInList dictionary, enter your plug-inʼs className, displayName, infoString, version,
and two different UUIDs, one for the plug-in, and one that matches the groupʼs UUID defined in ProPlugPlugInGroup
List. To avoid conflicts when the host loads plug-ins, every effect needs its own unique UUID, so generate a new one if
you duplicate an example project. Note that the protocolNames entry still requires either FxFilter or Fx
Generator, even though those classes are deprecated, so that host apps know what kind of effect your plug-in is.
Tip
Generate new UUIDs from the command line with the uuidgen command, which you can then copy and paste into
your plist.
There are some new keys to be added to this entry:
RunLoopType with a String value of “_NSApplication”,
JoinExistingSession with a Boolean value of YES, and
_AdditionalSubServices, a dictionary that contains a key “viewbridge” , and a Boolean value of YES.
Note
There are underscores within some of these key-value pairs, so add them with caution.
Your plug-in bundle may have a static list of plug-ins a host app should register, or it may leave registration to the
bundleʼs principal class. With a static list, plug-ins are recognized and loaded automatically; for a dynamic list, the
bundleʼs principal class is asked to register the plug-ins. The distinction depends on the value of the Boolean tag Pro
PlugDynamicRegistration in the property list. In the example below, the property list has listed YES for this key, so
the plug-in bundle will use dynamic registration.
If you use dynamic registration, you need to add additional key/values in your XPCʼs Info.plist PluginKit section. Add
the key ProPlugDynamicRegistrationPrincipalClass and make the value the name of the registrar class. Also
add a key for ProPlugDictionaryVersion to the XPCʼs Info.plist.
Dynamic registration incurs a plug-in scanning performance hit. Use static registration whenever possible. You can find
more information about dynamic registration in PROPlugInRegistering or <PluginManager/PROPlugInBundle
Registration.h>.
Building an FxPlug plug-in from an Xcode template
Create a plug-in in Xcode with the FxPlug template.
Building an FxPlug plug-in manually
Create your own plug-in in Xcode.
Using FxPlug APIs
Use various FxPlug APIs to communicate with host apps like Motion or Final Cut Pro.
Thread safety in plug-ins
Learn about reentrancy for plug-ins and best practices to make them thread safe.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
Add PlugInKit dictionary
Add ProPlugPlugInGroupList and ProPlugPlugInList keys
Add XPCService entry
Add ProPlugDynamicRegistration and
ProPlugDynamicRegistrationPrincipalClass keys
See Also
Plug-in fundamentals

Developer Documentation
Professional Video Applications/ FxPlug / Editing property lists for FxPlug plug-ins
Article
Editing property lists for FxPlug plug-ins
Modify the way hosts recognize and display your FxPlug plug-in.
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


