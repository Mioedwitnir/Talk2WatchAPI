import bb.cascades 1.2
import bb.system 1.2

TabbedPane {
    id: mainPane

    Tab {
        title: "Description"
        Page {  // descriptionPage
            ScrollView {    // scrollView
                Container { // scrollViewContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        text: "Description"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                    }        
                    
                    TextArea {
                        editable: false
                        text: "This app is the reference implementation of the Talk2Watch API and can be seen as a programming example for developers. It is meant to demonstrate the possibilities of interprocess communication with the Talk2Watch app."
                    }


                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Source Code"
                        onClicked: {
                            t2w.forwardSourceCode()
                            }
                    }
                                        
                    Container {
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}   
                        Label {
                            horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Center
                            text: "App integration"
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                        }
                        Button {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: "Details"
                            
                            onClicked: {
                                mainPane.activeTab = appIntegrationTab
                            }
                        }
                    }
                    TextArea {
                        editable: false
                        text: "A step-by-step authentication and action creation example\nNote: this feature requires the Pro version of Talk2Watch"
                    }
                    
                }   // scrollViewContainer
            }   // scrollView
        }   // descriptionPage
    }
   
    Tab {
        id: appIntegrationTab
        title: "App integration"
        Page {  // page
            ScrollView {
                Container { // mainPageContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        text: "App integration"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                    }
                    

                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: "App title"
                            preferredWidth: 250
                        }
                        TextField {
                            verticalAlignment: VerticalAlignment.Center
                            id: appTitleField
                            text: "NewApp"
                        }     
                    }   
                    TextArea {
                        editable: false
                        topPadding: 10
                        text: "The title should be a unique identifier for the app."
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: "App version"
                            preferredWidth: 250
                        }
                        TextField {
                            verticalAlignment: VerticalAlignment.Center
                            id: appVersionField
                            text: "1.0"
                        }     
                    }         
                    TextArea {
                        editable: false
                        topPadding: 10
                        text: "A version number for the app integration part. It is used as an indicator to find out which actions have already been created."
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: "App key"
                            preferredWidth: 250
                        }
                        TextField {
                            verticalAlignment: VerticalAlignment.Center
                            id: appKeyField
                            text: "df78962534Sad2378ewasd"
                        }     
                    }       
                    TextArea {
                        editable: false
                        topPadding: 10
                        text: "A unique sequence to protect your app against modifications from others. Ensure that you always you the same key for your app!"
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: "App description"
                            preferredWidth: 250
                        }
                        TextArea {
                            verticalAlignment: VerticalAlignment.Center
                            id: appDescriptionTextArea
                            text: "This is a new app"
                        }
                    }     
                    TextArea {
                        editable: false
                        topPadding: 10
                        text: "The 'Description' will only be displayed on the Script page and should give some more information about the app. Also a link to the app in BlackBerry world can be added here."
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: "Auth status"
                            preferredWidth: 250
                        }
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            objectName: "m_authStatusLabel"
                            text: "AUTH_UNKNOWN"
                            
                            onTextChanged: {
                                if(text=="AUTH_SUCCESS")
                                    scriptCreationContainer.setVisible(true)
                                else if(text=="AUTH_KEY_MISMATCH")
                                    scriptCreationContainer.setVisible(false)
                            }
                        }    
                    }                    
                    Button {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Request AUTH"
                        onClicked: {
                            t2w.setAppValues(appTitleField.text, appVersionField.text, appKeyField.text, "UDP", "8484", appDescriptionTextArea.text);
                            t2w.sendAppAuthorizationRequest();
                        }
                    }
                    Button {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Remove Connection"
                        onClicked: {
                            t2w.removeConnection();
                        }
                    }
                    


                    
                    
                    Container {
                        topPadding: 50
                        visible: false
                        id: scriptVerificationContainer
                        
                        Label {
                            text: "Step 3: Verification (optional)"
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    
                    bottomPadding: 50
                }    //mainPageContainer
            }
        }   // page
    }
    
    
    
    Tab {
        title: "Actions"
        Page {  // page
            ScrollView {
                Container { // mainPageContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        text: "Actions"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        verticalAlignment: VerticalAlignment.Center
                    }

                    Container {
                        id: scriptCreationContainer
                        
 
                        
                        Container {
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Title"
                                preferredWidth: 250
                            }
                            TextField {
                                verticalAlignment: VerticalAlignment.Center
                                id: scriptTitleTextField
                                text: "NewAction"
                            }     
                        }   
                        TextArea {
                            editable: false
                            topPadding: 10
                            text: "The 'Title' will be displayed in the action list when the user uses ScriptMode."
                        }
                        Container {
                            topPadding: 10
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Folder"
                                preferredWidth: 250
                            }
                            TextField {
                                verticalAlignment: VerticalAlignment.Center
                                id: scriptFolderTextField
                                text: ""
                                hintText: "optional"
                            }     
                        }  
                        Container {
                            topPadding: 10
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Command"
                                preferredWidth: 250
                            }
                            TextField {
                                verticalAlignment: VerticalAlignment.Center
                                id: scriptCommandTextField
                                text: "NewCommand"
                            }     
                        }   
                        TextArea {
                            editable: false
                            topPadding: 10
                            text: "The 'Command' is the value which will be sent to back to this app when the user triggers the action."
                        }
                        Container {
                            topPadding: 10
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Description"
                                preferredWidth: 250
                            }
                            TextArea {
                                verticalAlignment: VerticalAlignment.Center
                                id: actionDescriptionTextArea
                                text: "This is a new action"
                            }
                        }  
                        TextArea {
                            editable: false
                            topPadding: 10
                            text: "The 'Description' will only be displayed on the Script page and should give some more information about the action."
                        }
                        Button {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            text: "Create action"
                            onClicked: {
                                t2w.createAction(scriptTitleTextField.text, scriptFolderTextField.text, scriptCommandTextField.text, actionDescriptionTextArea.text);
                            }
                        }    
                        Button {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            text: "Delete action"
                            onClicked: {
                                t2w.removeAction(scriptTitleTextField.text);
                            }
                        }                    
                    }
                    
                    
                    
                    bottomPadding: 50
                
                }    //mainPageContainer
            }
        }   // page
    
    }   // Tab
    Tab {
        title: "Sending Messages"
        Page {  // page
            ScrollView {
                Container { // mainPageContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        objectName: "m_label"
                        text: "Sending Messages"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        verticalAlignment: VerticalAlignment.Center
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            text: "Title"
                            preferredWidth: 250
                            verticalAlignment: VerticalAlignment.Center
                        }
                        
                        TextField {
                            id: messageTitleField
                            verticalAlignment: VerticalAlignment.Center
                            text: "Hello"
                        }
                    
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            text: "Text"
                            preferredWidth: 250
                            verticalAlignment: VerticalAlignment.Center
                        }
                        
                        TextField {
                            id: messageTextField
                            verticalAlignment: VerticalAlignment.Center
                            text: "Hello"
                        }
                    
                    }
                    
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Send"
                        verticalAlignment: VerticalAlignment.Center
                        onClicked: {
                            t2w.sendSms(messageTitleField.text, messageTextField.text)
                        }
                    }
                    bottomPadding: 50
                
                }    //mainPageContainer
            }
        }   // page
    
    }   // Tab

Tab {
        title: "Folders"
        Page {  // page
            ScrollView {
                Container { // mainPageContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        text: "Folders"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        verticalAlignment: VerticalAlignment.Center
                    }
                    Container {
                        topPadding: 50
                        //visible: false
                        id: folderCreationContainer
                        
                        Label {
                            text: "Folder creation"
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                        }
                        
                        Container {
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Title"
                                preferredWidth: 250
                            }
                            TextField {
                                verticalAlignment: VerticalAlignment.Center
                                id: folderTitleTextField
                                text: "NewFolder"
                            }     
                        }   
                        TextArea {
                            editable: false
                            topPadding: 10
                            text: "The 'Title' will be displayed in the action list when the user uses ScriptMode."
                        }
                        Container {
                            layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "Parent"
                                preferredWidth: 250
                            }
                            TextField {
                                verticalAlignment: VerticalAlignment.Center
                                id: folderParentTextField
                                text: ""
                                hintText: "optional"
                            }     
                        }   
                        TextArea {
                            editable: false
                            topPadding: 10
                            text: "The 'Title' will be displayed in the action list when the user uses ScriptMode."
                        }
                        Button {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            text: "Create folder"
                            onClicked: {
                                if(folderParentTextField.text=="")
                                    t2w.createFolder(folderTitleTextField.text);
                                else 
                                    t2w.createFolder(folderTitleTextField.text, folderParentTextField.text);
                            }
                        }       
                        Button {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            text: "Delete folder"
                            onClicked: {
                                t2w.removeFolder(folderTitleTextField.text)
                            }
                        }               
                    }
                    
                    
                    
                    bottomPadding: 50
                
                }    //mainPageContainer
            }
        }   // page
    
    }   // Tab

    Tab {
        title: "App Message"
        Page {  // page
            ScrollView {
                Container { // mainPageContainer
                    leftPadding: 30
                    rightPadding: 30
                    Label {
                        objectName: "m_label"
                        text: "App Message"
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        verticalAlignment: VerticalAlignment.Center
                    }
                    Container {
                        topPadding: 10
                        layout: StackLayout {orientation: LayoutOrientation.LeftToRight}         
                        Label {
                            text: "UUID"
                            preferredWidth: 250
                            verticalAlignment: VerticalAlignment.Center
                        }
                        
                        TextField {
                            id: uuidTitleField
                            verticalAlignment: VerticalAlignment.Center
                            text: ""
                        }
                    
                    }

                    
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Register"
                        verticalAlignment: VerticalAlignment.Center
                        onClicked: {
                            t2w.registerAppMessageListener(uuidTitleField.text)
                        }
                    }
                    
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Deregister"
                        verticalAlignment: VerticalAlignment.Center
                        onClicked: {
                            t2w.deregisterAppMessageListener(uuidTitleField.text)
                        }
                    }
                    bottomPadding: 50
                
                }    //mainPageContainer
            }
        }   // page
    
    }   // Tab
}