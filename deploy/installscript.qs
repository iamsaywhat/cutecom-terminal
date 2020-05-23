function Component()
{
    // default constructor
}
Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
//        if (installer.value("os") == "win") {
        if (systemInfo.productType === "windows") {
            try {
                var userProfile = installer.environmentVariable("USERPROFILE");
                installer.setValue("UserProfile", userProfile);
                component.addOperation("CreateShortcut",
                                       "@TargetDir@/cutecom.exe",                        // target
                                       "@DesktopDir@/CuteCOM.lnk",                       // link-path
                                       "workingDirectory=@TargetDir@",                   // working-dir
                                       "iconPath=@TargetDir@/cutecom.exe", "iconId=0",   // icon
                                       "description=Start App");                         // description
                    component.addOperation("CreateShortcut",
                                           "@TargetDir@/cutecom.exe",                        // target
                                           "@StartMenuDir@/CuteCOM.lnk",                     // link-path
                                           "workingDirectory=@TargetDir@",                   // working-dir
                                           "iconPath=@TargetDir@/cutecom.exe", "iconId=0",   // icon
                                           "description=Start App");                         // description
            } catch (e) {
                // Do nothing if key doesn't exist
            }
        }
    } catch (e) {
        print(e);
    }
}
