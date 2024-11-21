

function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();    
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/micro_brico_app.exe", "@StartMenuDir@/micro_brico_app.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll",
            "iconId=2", "description=Execute microBrico appli.");
    }
}