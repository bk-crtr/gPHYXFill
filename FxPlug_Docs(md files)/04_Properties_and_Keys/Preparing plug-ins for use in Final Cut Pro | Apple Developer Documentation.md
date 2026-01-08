# DOCUMENTATION SOURCE: Preparing plug-ins for use in Final Cut Pro | Apple Developer Documentation.pdf

To make FxPlug plug-ins work in Final Cut Pro, you need to create a Final Cut Effect within Motion that rigs and/or
publishes your FxPlug parameters. For more information, see Rigging overview in Motion Help and Final Cut Pro
Templates Overview.
Note
Final Cut Pro for iPad doesnʼt support FxPlug 4 plug-ins.
Effects in Final Cut Pro include Generators, Transitions, and Filters. These Final Cut Pro effects are really just Motion
documents, which can contain whatever you choose. This is useful because most users of Final Cut Pro want to solve
specific tasks, rather than to apply particular effects with dozens of parameters. Advanced users who need more control
will be able to either open your Final Cut Effect in Motion and edit it themselves, or create their own Final Cut Pro Effects
within Motion for their specific task.
Note
In general, plug-in users donʼt need control over every aspect of a plug-in to get the effect; effects that offer too
many parameters to the user can be overwhelming.
To prepare your plug-in you use Motion to create a Final Cut Effect, Generator, Transition, or Title. To do that, youʼll need
to create a Motion Effect, apply your filter or generator, and publish its parameters in a rig. A rig consolidates your plug-
inʼs parameters, which you can then publish in your effect. This makes it easier for Final Cut Pro users to apply the
effects of your plug-in to their projects. (For information about creating rigs for your plug-in, see Rigging overview in
Motion Help).
The following steps outline the workflow for creating a basic Final Cut Pro Effect in Motion. For details about any step or
about using Motion, see Motion Help.
P. Launch Motion and select the Final Cut Pro Effect template from the Project Browser. This creates a new Motion
project with a placeholder layer called Effect Source.
T. Apply the Glow filter. You can do this by dragging the effect out of the filters library and onto the Effect Source
placeholder. Alternatively, you can select the Effect Source placeholder layer, click Filters and choose Glow, and
finally click Apply. Save your effect.
V. Open the filterʼs Inspector. Select the newly applied Glow plug-in and open the Motion inspector to reveal the plug-
inʼs parameters.
W. Build and name a rig to connect the controls for the Radius parameter. Click the popup-menu indicator to the right of
the Radius slider to see the options for the Radius parameter. Choose Add To Rig > Create New Rig > Add To New
Slider. Motion creates a new Rig group in the layer list with the new layer (named Slider) selected. Change the name
of the slider to Glowiness. Finally, choose Publish from the slider parameterʼs option menu.
Z. Connect the rig to the Opacity parameter. In the layer list, select the Glow filter to see its parameters in the Inspector.
Parameters with a joystick icon indicate theyʼre controlled with a rig—notice that the Radius parameter shows this
icon. Now, add another parameter to this rig. Click the pop-up menu indicator to the right of the Opacity slider and
choose Add To Rig > Rig > Add To Glowiness.
\. Set the low end of parameters for the Glowiness effect. Select the rig named Glowiness in the layer list. In the
inspector, youʼll see a slider that controls the parameter levels. On this slider, click the circle at the left end. Edit the
levels for when Glowiness is set to zero. There are two parameters under this rigʼs control: Opacity, and Radius. Set
the Effect Source.Opacity and Effect Source.Radius sliders to 0. This sets the Opacity and Radius parameters to 0
when the rigged slider is set to 0.
_. Set the high end of parameters for the Glowiness effect. Now edit the right end of the rig. Begin again by clicking the
circle at the right end of the Glowiness parameter. Next, set the Effect Source.Opacity and Effect Source.Radius
sliders to their maximum values. You now have a rigged parameter. Drag the Glowiness parameter and verify that the
underlying parameters change as well.
When you apply this plug-in in Final Cut Pro, users of the effect see a single slider (called Glowiness) which they can use
to control both the Radius and Opacity parameters.
If you want to give your users direct access so they can control every aspect of a parameter in your plug-in, publish
each parameter in Motion directly, without a rig. Unlike rigging a parameter, publishing a parameter gives users
individual control over a single parameter.
To create the Threshold parameter for direct manipulation:
P. Show the filterʼs parameters in the inspector. In the layer list, select the Glow effect and be sure the Inspector tab is
visible.
T. Publish the Threshold parameter. In the inspector, click the Threshold parameterʼs pop- up menu, and choose
Publish.
V. Save the effect. When you attempt to save a Final Cut Pro Effect, Transition, Generator, or Title, youʼre presented with
some options. In addition to a name, you can assign the effect to a category, which is where it appears in the Final Cut
Pro Effect browser. You can also add it to a theme. After you have a name and category, click the Publish button.
Your effect is immediately available in Final Cut Pro.
If your Motion Template doesnʼt create a temporal effect (a timing effect such as retiming, temporal blur, or
echo/feedback), set the Project Loop End marker in the template. If you donʼt include such a marker, your effects will
rerender after a blade, trim, or edit in Final Cut Pro. For more about Project Loop End markers, see What are template
markers?
When you update an existing plug-in, you may also need to deprecate the older version of the Motion template. A
deprecated template is considered obsolete and does not appear in the effects browser in Motion, but it will still be
applied in projects that previously used it.
The second bit of the <flags> value in the Motion XML for a template indicates whether or not the template has been
deprecated. A 0 value for the second bit indicates that the template has not been deprecated; a value of 1 indicates that
it has been deprecated.
To deprecate a template, open the Motion XML document and take the current value of the <flags> element (a child of
the <template> element) and perform a bitwise OR operation with the value 0x2. For example, if the value of <flags>
was previously 1, it would now be 3:
Note
If a project isnʼt a Designed for 4K project, then the <template> element wonʼt exist in the XML document. To
deprecate this type of project, manually add the <template> element and set <flags> to 2.
Testing FxPlug plug-ins
Test and debug FxPlug plug-ins using a variety of methods.
Notarizing your FxPlug plug-in
Give users confidence in your FxPlug plug-in by enabling notarization.
To submit feedback on documentation, visit Feedback Assistant. Light Dark Auto
Copyright © 2025 Apple Inc. All rights reserved. Terms of Use Privacy Policy Agreements and Guidelines
All Technologies
Professional Video Applications
No data available.
/Filter
Overview
Create a basic Final Cut Pro effect
Publish the parameters for your plug-in
Set temporal effects and project loop markers
Deprecate a Motion template
<template>
    <flags>3</flags>
</template>
See Also
Te s t i n g  a n d  d e p l o y m e n t

Developer Documentation
Professional Video Applications/ FxPlug / Preparing plug-ins for use in Final Cut Pro
Article
Preparing plug-ins for use in Final Cut Pro
Add a plug-in to a Motion effect template to use in Final Cut Pro.
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


