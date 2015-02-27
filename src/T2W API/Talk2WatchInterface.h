/*
 * Talk2WatchInterface.h
 *
 *  Created on: 09.01.2014
 *      Author: benjaminsliwa
 */

#ifndef TALK2WATCHINTERFACE_H_
#define TALK2WATCHINTERFACE_H_

#include <QObject>
#include <QVariantList>
#include <bb/system/InvokeManager>

class Serializer;
class UdpModule;

class Talk2WatchInterface : public QObject
{
	Q_OBJECT
public:

	/************************************************************
	 * 							SETUP							*
	 ***********************************************************/

	/*	Add the following Libs to your project
	 * 	LIBS: libbbsystem
	 *
	 *	Create an a new Talk2WatchInterface object
	 */

	Talk2WatchInterface(QObject *_parent = 0);

	/*	HANDLING DIFFERENT TALK2WATCH VERSIONS
	 *
	 *	As the normal Talk2Watch version and the Pro version use different target ids the installed version has to be determined.
	 *	On object creation a lookup will be performed automatically. The transmission methods will automatically chose the right app.
	 *	If both apps are installed, the Pro version will be chosen.
	 *
	 *	Note: the initial lookup takes some time - if you call a send method directly after object creation it will probably fail
	 */

	virtual ~Talk2WatchInterface();

	/************************************************************
	 * 						HELPER METHODS						*
	 ***********************************************************/

	Q_INVOKABLE bool isTalk2WatchInstalled();

	Q_INVOKABLE bool isTalk2WatchProInstalled();

	Q_INVOKABLE bool isTalk2WatchProServiceInstalled();


	/************************************************************
	 * 		SENDING MESSAGES TO THE WATCH VIA TALK2WATCH		*
	 ***********************************************************/

	/*
	 *	Currently there are two possibilities of displaying a message on the watch - SMS or Email.
	 * 	The only difference is the number of available variables (-> different text areas on the watch).
	 *
	 * 	Sending messages does not require any setup methods
	 */

	Q_INVOKABLE void sendSms(QString _sender, QString _text);

	Q_INVOKABLE void sendEmail(QString _sender, QString _subject, QString _text);


	/************************************************************
	 * 		RECEIVING ACTIONS FROM THE WATCH VIA TALK2WATCH		*
	 ***********************************************************/

	/*	NOTE: The message receiving part is only available for Talk2Watch Pro	 */


	/*	SETUP METHODS - DESCRIPTION
	 *
	 *	Every application which wants to receives messages from Talk2Watch has to setup a receiver first.
	 *
	 *	Messages can be received via
	 *	- TCP/ UDP
	 */

	/*	RECEIVING MESSAGES - STEP 1
	 *
	 *	Chose one of the available receiver models, implement it and call the activation method with the correct parameters
	 */

	Q_INVOKABLE void setAppValues(QString _appName, QString _appVersion, QString _appKey, QString _protocol, QString _port, QString _description);


	/*	APPLICATION AUTHORIZATION
	 *
	 *	For receiving notifications about triggered actions every application has to be authorized by the user.
	 *	If the user grants the requested permissions Talk2Watch will create a new folder on the "Script" page for the application.
	 */

	/*	RECEIVING MESSAGES - STEP 2
	 *
	 *	Call the authorization method and wait for a reply. The reply will be passed to the receiver which was defined in STEP 1.
	 *	If the request was successful the value AUTH_SUCCESS will be returned
	 */

	Q_INVOKABLE void sendAppAuthorizationRequest();

	/*	SCRIPT CREATION
	 *
	 *	After a successful authorization the application is allowed to create new actions. Every action will be added to the new folder which
	 *	was created in STEP 2.
	 *
	 *	An action consists of three parts
	 *	1) TITLE: the title will be displayed on the watch when the ScriptMode is active
	 *	2) COMMAND:	the value which is passed back to the application receiver when a script is triggered
	 *	3) DESCRIPTION: the text which will be shown when the user selects the action from the application folder on the "Script" page
	 *
	 */

	/*	EXAMPLE
	 *
	 *	A location based applications wants to save the users position via a Talk2Watch script which is triggered from the watch
	 *
	 * 	TITLE:			Save position
	 * 	COMMAND:		SAVE_POSITION
	 *	DESCRIPTION:	Uses GPS to get the current latitude and logitude values and saves them.
	 */

	/*	RECEIVING MESSAGES - STEP 3
	 *
	 *	Use the creator methods to create the actions you want to add to Talk2Watch.
	 */

	Q_INVOKABLE void createAction(QString _title, QString _command, QString _description);

	/*  IMPORTANT NOTE
	 *  The following methods are only working with the new beta version of Talk2Watch Pro
	 *
	 */

	Q_INVOKABLE void createAction(QString _title, QString _folder, QString _command, QString _description);

	Q_INVOKABLE void createFolder(QString _title);

	Q_INVOKABLE void createFolder(QString _title, QString _parentFolder);

	Q_INVOKABLE void removeConnection();

	Q_INVOKABLE void removeAction(const QString &_action);

	Q_INVOKABLE void removeFolder(const QString &_folder);

    Q_INVOKABLE void renameAction(const QString &_oldTitle, const QString &_newTitle);

    Q_INVOKABLE void renameFolder(const QString &_oldTitle, const QString &_newTitle);

	Q_INVOKABLE void forwardSourceCode();


    /************************************************************
     *                      PEBBLE METHODS                      *
     ***********************************************************/

	/*  PEBBLE APP MESSAGE
	 *
	 *  App messages can be sent with specifying their numeric app key values (as they are defined in the appinfo file) as strings with the corresponding data values as QVariant
	 */

	/*  EXAMPLE
	 *
	 *  You want to send an integer temperature value to a watchface (the uuid has previously been defined and stored in the uuid variable).
	 *  You have defined an endpoint TEMPERATURE in your appinfo file and it is mapped to 1.
	 *  Then the app message call looks like this: (for a temperature value = 30)
	 *
	 *  QHash<QString, QVariant> values;
	 *  values.insert("1", 30);
	 *
	 *  sendAppMessage(uuid, values);
	 */

	Q_INVOKABLE void sendAppMessage(const QString &_uuid, const QHash<QString, QVariant> &_values);

	Q_INVOKABLE void sendAppLaunchRequest(const QString &_uuid);

    /*  PEBBLE EVENTS
     *
     *  Registering a pebble event listener requires an authorized app connection (see the methods above).
     *  After the registration method has been called, T2W sends back REGISTER_UUID_SUCCESS if everything went fine
     *
     *  From that point of time, this class will emit the following signals as soon as their corresponding event occurs in T2W
     *
     *  void appMessageReceived(const QString &_uuid, const QHash<QString, QVariant> &_values);
     *  void appStarted(const QString &_uuid);
     *  void appClosed(const QString &_uuid);
     */


    Q_INVOKABLE void registerAppMessageListener(const QString &_uuid);

    Q_INVOKABLE void deregisterAppMessageListener(const QString &_uuid);

	void handleMessage(const QString &_type, const QString &_category, const QHash<QString, QVariant> &_values);


private:

	// UDP
	UdpModule *m_udp;
	Serializer *m_serializer;

	// Transmission
	void sendCommand(QString _command);
	void sendCommandViaInvocation(QString _command, QString _target);
	void sendAuthenticatedCommand(const QString &_type, const QString &_category, const QStringList &_keys, const QVariantList &_values);

	QString m_appName;
	QString m_appVersion;
	QString m_appKey;
	QString m_protocol;
	QString m_port;
	QString m_description;

	QString m_target;
	QString m_mimeType;

	bb::system::InvokeManager *m_invokeManager;


	bool m_talk2WatchAvailable;
	bool m_talk2WatchProAvailable;
	bool m_talk2WatchProServiceAvailable;



signals:
	void transmissionReady();
	void receivedData(QString); // DEPRECATED - use the signals below

	void authSuccess();
	void authError(const QString &_error);
	void actionCreationSuccess(const QString &_action);
	void actionCreationError(const QString &_action, const QString &_error);
	void actionRemovalSuccess(const QString &_action);
	void actionRemovalError(const QString &_action, const QString &_error);
	void actionRenamingSuccess(const QString &_action);
	void actionRenamingError(const QString &_action, const QString &_error);
	void actionTriggered(const QString &_command);
	void connectionRemovalSuccess();
	void folderCreationSuccess(const QString &_folder);
	void folderCreationError(const QString &_folder, const QString &_error);
	void folderRemovalSuccess(const QString &_folder);
	void folderRemovalError(const QString &_folder, const QString &_error);
	void folderRenamingSuccess(const QString &_folder);
	void folderRenamingError(const QString &_folder, const QString &_error);
	void uuidRegistrationSuccess(const QString &_uuid);
	void uuidDeregistrationSucess(const QString &_uuid);


	void appMessageReceived(const QString &_uuid, const QHash<QString, QVariant> &_values);
	void appStarted(const QString &_uuid);
	void appClosed(const QString &_uuid);

public slots:
	void onDataReived(const QString &_data);
	void onTalk2WatchLookup();


};

#endif /* TALK2WATCHINTERFACE_H_ */
