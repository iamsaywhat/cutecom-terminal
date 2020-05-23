Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
        if (installer.value("os") == "win") { 
            try {
                var userProfile = installer.environmentVariable("USERPROFILE");
                installer.setValue("UserProfile", userProfile);
                component.addOperation("CreateShortcut", "@TargetDir@\\MiamPlayer.exe", "@UserProfile@\\Desktop\\MiamPlayer.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
            }
        }
    } catch (e) {
        print(e);
    }
}