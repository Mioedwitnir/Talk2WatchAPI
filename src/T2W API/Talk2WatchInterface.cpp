/*
 * Talk2WatchInterface.cpp
 *
 *  Created on: 09.01.2014
 *      Author: benjaminsliwa
 */

#include "Talk2WatchInterface.h"
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeReply>
#include "UdpModule.h"
#include "Serializer.h"

/*	GENERAL INFORMATION
 *
 *
 *
 */

Talk2WatchInterface::Talk2WatchInterface(int _udpPort, QObject *_parent)
{

	m_udp = new UdpModule(this);

	if(_udpPort!=-1)
	{
	    m_udp->listenOnPort(_udpPort);
	    connect(m_udp, SIGNAL(reveivedData(QString)), this, SLOT(onDataReived(QString)));
	}

	m_serializer = new Serializer(this);


	m_talk2WatchAvailable = false;
	m_talk2WatchProAvailable = false;
	m_talk2WatchProServiceAvailable = false;

	m_invokeManager = new bb::system::InvokeManager(this);

	m_appName = "";
	m_appKey = "";
	m_description = "";
	m_protocol = "";
	m_port = "";
	m_target = "";
	m_mimeType = "";


	bb::system::InvokeQueryTargetsRequest request;
	request.setMimeType ("text/plain");
	request.setAction("bb.action.SHARE");
	const bb::system::InvokeQueryTargetsReply *reply = m_invokeManager->queryTargets(request);


	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(onTalk2WatchLookup()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

Talk2WatchInterface::~Talk2WatchInterface()
{

}

/************************************************************
 * 						HELPER METHODS						*
 ***********************************************************/

bool Talk2WatchInterface::isTalk2WatchInstalled()
{
	return m_talk2WatchAvailable;
}

bool Talk2WatchInterface::isTalk2WatchProInstalled()
{
	return m_talk2WatchProAvailable;
}

bool Talk2WatchInterface::isTalk2WatchProServiceInstalled()
{
	return m_talk2WatchProServiceAvailable;
}

/************************************************************
 * 		SENDING MESSAGES TO THE WATCH VIA TALK2WATCH		*
 ***********************************************************/

void Talk2WatchInterface::sendSms(QString _sender, QString _text)
{
    QStringList keys = QStringList() << "text" << "sender";
    QVariantList values = QVariantList() << _text << _sender;
    sendCommand(m_serializer->serialize("SMS", "NOTIFICATIONS", keys, values));
}

void Talk2WatchInterface::sendEmail(QString _sender, QString _subject, QString _text)
{
    QStringList keys = QStringList() << "text" << "sender" << "subject";
    QVariantList values = QVariantList() << _text << _sender << _subject;
    sendCommand(m_serializer->serialize("EMAIL", "NOTIFICATIONS", keys, values));
}

/************************************************************
 * 		RECEIVING ACTIONS FROM THE WATCH VIA TALK2WATCH		*
 ***********************************************************/

/*	RECEIVING MESSAGES - STEP 1
 *
 *	Chose one of the available receiver models, implement it and call the activation method with the correct parameters
 */

void Talk2WatchInterface::setAppValues(QString _appName, QString _appVersion, QString _appKey, QString _protocol, QString _port, QString _description)
{
	m_appName = _appName;
	m_appVersion = _appVersion;
	m_appKey = _appKey;
	m_protocol = _protocol;
	m_port = _port;
	m_description = _description;
}


/*	RECEIVING MESSAGES - STEP 2
 *
 *	Call the authorization method and wait for a reply. The reply will be passed to the receiver which was defined in STEP 1.
 *	If the request was successful the value APP_RQ_SUCCESS will be returned
 */

void Talk2WatchInterface::sendAppAuthorizationRequest()
{
    QStringList keys = QStringList() << "appVersion" << "port" << "description";
    QVariantList values = QVariantList() << m_appVersion << m_port << m_description;
    sendAuthenticatedCommand("APP_AUTH_RQ", "APP_CONNECTION", keys, values);
}

/*	RECEIVING MESSAGES - STEP 3
 *
 *	Use the creator methods to create the actions you want to add to Talk2Watch.
 */

void Talk2WatchInterface::createAction(QString _title, QString _command, QString _description)
{
    QStringList keys = QStringList() << "title" << "command" << "description";
    QVariantList values = QVariantList() << _title << _command << _description;
    sendAuthenticatedCommand("APP_CREATE_ACTION", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::createAction(QString _title, QString _folder, QString _command, QString _description)
{
    QStringList keys = QStringList() << "title" << "folder" << "command" << "description";
    QVariantList values = QVariantList() << _title << _folder << _command << _description;
    sendAuthenticatedCommand("APP_CREATE_ACTION", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::createFolder(QString _title)
{
    createFolder(_title, "root");
}

void Talk2WatchInterface::createFolder(QString _title, QString _parentFolder)
{
    QStringList keys = QStringList() << "title" << "parentFolder";
    QVariantList values = QVariantList() << _title << _parentFolder;
    sendAuthenticatedCommand("APP_CREATE_FOLDER", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::removeConnection()
{
    QStringList keys;
    QVariantList values;
    sendAuthenticatedCommand("APP_REMOVE_CONNECTION", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::removeAction(const QString &_action)
{
    QStringList keys = QStringList() << "action";
    QVariantList values = QVariantList() << _action;
    sendAuthenticatedCommand("APP_REMOVE_ACTION", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::removeFolder(const QString &_folder)
{
    QStringList keys = QStringList() << "folder";
    QVariantList values = QVariantList() << _folder;
    sendAuthenticatedCommand("APP_REMOVE_FOLDER", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::renameAction(const QString &_oldTitle, const QString &_newTitle)
{
    QStringList keys = QStringList() << "oldTitle" << "newTitle";
    QVariantList values = QVariantList() << _oldTitle << _newTitle;
    sendAuthenticatedCommand("APP_RENAME_ACTION", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::renameFolder(const QString &_oldTitle, const QString &_newTitle)
{
    QStringList keys = QStringList() << "oldTitle" << "newTitle";
    QVariantList values = QVariantList() << _oldTitle << _newTitle;
    sendAuthenticatedCommand("APP_RENAME_FOLDER", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::forwardSourceCode()
{
	bb::system::InvokeRequest request;
	request.setTarget("sys.pim.uib.email.hybridcomposer");
	request.setAction("bb.action.SHARE");
	request.setUri("file:///" + QDir::currentPath() + "/app/native/assets/T2W_API.zip");
	m_invokeManager->invoke(request);
}

void Talk2WatchInterface::sendAppMessage(const QString &_uuid, const QHash<QString, QVariant> &_values)
{
    QHash<QString, QVariant> values = _values;
    values.insert("uuid", _uuid);

    sendCommand(m_serializer->serialize("APPMESSAGE", "PEBBLE", values));
}

void Talk2WatchInterface::sendAppLaunchRequest(const QString &_uuid)
{
    QHash<QString, QVariant> values;
    values.insert("uuid", _uuid);

    sendCommand(m_serializer->serialize("LAUNCH_APP", "PEBBLE", values));
}

void Talk2WatchInterface::registerAppMessageListener(const QString &_uuid)
{
    QStringList keys = QStringList() << "uuid";
    QVariantList values = QVariantList() << _uuid;
    sendAuthenticatedCommand("APP_REGISTER_UUID", "APP_CONNECTION", keys, values);
}

void Talk2WatchInterface::deregisterAppMessageListener(const QString &_uuid)
{
    QStringList keys = QStringList() << "uuid";
    QVariantList values = QVariantList() << _uuid;
    sendAuthenticatedCommand("APP_DEREGISTER_UUID", "APP_CONNECTION", keys, values);
}


/************************************************************
 * 						PRIVATE METHODS						*
 ***********************************************************/

// Transmission
void Talk2WatchInterface::sendCommand(QString _command)
{
	if(m_talk2WatchProServiceAvailable || m_talk2WatchProAvailable==true)
		m_udp->sendMessage("127.0.0.1", 9877, _command);
	else if(m_talk2WatchAvailable==true)
		sendCommandViaInvocation(_command, "com.Talk2Watch.invocation.msg");
	else
	{
		qDebug() << "T2W not found";
	}

}

void Talk2WatchInterface::sendCommandViaInvocation(QString _command, QString _target)
{
    bb::system::InvokeRequest request;
    request.setTarget(_target.toStdString().c_str());
    request.setAction("bb.action.SHARE");
    request.setData(_command.toStdString().c_str());
    request.setMimeType("text/plain");

    m_invokeManager->invoke(request);
}

void Talk2WatchInterface::sendAuthenticatedCommand(const QString &_type, const QString &_category, const QStringList &_keys, const QVariantList &_values)
{
    QStringList keys = QStringList() << "appName" << "appKey" << _keys;
    QVariantList values = QVariantList() << m_appName << m_appKey << _values;
    sendCommand(m_serializer->serialize(_type, _category, keys, values));
}

void Talk2WatchInterface::handleMessage(const QString &_type, const QString &_category, const QHash<QString, QVariant> &_values)
{
    if(_category=="PEBBLE")
    {
        if(_type=="APPMESSAGE_RECEIVED")
        {
            QHash<QString, QVariant> values = _values;
            QString uuid = _values.value("uuid").toString();
            values.remove("uuid");

            emit appMessageReceived(uuid, values);
        }
        else if(_type=="APP_STARTED")
            emit appStarted(_values.value("uuid").toString());
        else if(_type=="APP_CLOSED")
            emit appClosed(_values.value("uuid").toString());
    }
    else if(_category=="PEBBLE_NOTIFICATIONS")
    {
        qDebug() << "__EVENT__" << _type; //<< _values;

        QString id = _values.value("id").toString();
        QString text = _values.value("text").toString();

        if(_type=="DISMISS")
            emit dismissReceived(id);
        else if(_type=="GENERIC")
            emit genericReceived(id, text);
        else if(_type=="RESPONSE")
            emit responseReceived(id, text);

    }
    else if(_category=="APP_CONNECTION")
    {
        qDebug() << "__RX__" << _type << _values;

        QString action = _values.value("action").toString();
        QString error = _values.value("error").toString();
        QString folder = _values.value("folder").toString();
        QString uuid = _values.value("uuid").toString();

        if(_type=="AUTH_SUCCESS")
            emit authSuccess();
        else if(_type=="AUTH_ERROR")
            emit authError(error);
        else if(_type=="CREATE_ACTION_SUCCESS")
            emit actionCreationSuccess(action);
        else if(_type=="CREATE_ACTION_ERROR")
            emit actionCreationError(action, error);
        else if(_type=="REMOVE_ACTION_SUCCESS")
            emit actionRemovalSuccess(action);
        else if(_type=="REMOVE_ACTION_ERROR")
            emit actionRemovalError(action, error);
        else if(_type=="RENAME_ACTION_SUCCESS")
            emit actionRenamingSuccess(action);
        else if(_type=="RENAME_ACTION_ERROR")
            emit actionRenamingError(action, error);
        else if(_type=="ACTION_TRIGGERED")
            emit actionTriggered(_values.value("command").toString());
        else if(_type=="REMOVE_CONNECTION_SUCCESS")
            emit connectionRemovalSuccess();
        else if(_type=="CREATE_FOLDER_SUCCESS")
            emit folderCreationSuccess(folder);
        else if(_type=="CREATE_FOLDER_ERROR")
            emit folderCreationError(folder, error);
        else if(_type=="REMOVE_FOLDER_SUCCESS")
            emit folderRemovalSuccess(folder);
        else if(_type=="REMOVE_FOLDER_ERROR")
            emit folderRemovalError(folder, error);
        else if(_type=="RENAME_FOLDER_SUCCESS")
            emit folderRenamingSuccess(folder);
        else if(_type=="RENAME_FOLDER_ERROR")
            emit folderRenamingError(folder, error);
        else if(_type=="REGISTER_UUID_SUCCESS")
            emit uuidRegistrationSuccess(uuid);
        else if(_type=="DEREGISTER_UUID_SUCCESS")
            emit uuidDeregistrationSucess(uuid);
    }
}

QString Talk2WatchInterface::sendPebbleNotification(PebbleNotification _notification)
{
    QHash<QString, QVariant> values = _notification.getValues();

    sendAuthenticatedCommand("SEND_NOTIFICATION", "PEBBLE", values.keys() << "appId", values.values() << "127.0.0.1:" + m_port);

    return _notification.getId();
}

void Talk2WatchInterface::sendPebbleNotificationAcknowledgment(const QString &_id, const QString &_text)
{
    QStringList keys = QStringList() << "id" << "text";
    QVariantList values = QVariantList() << _id << _text;
    sendCommand(m_serializer->serialize("SEND_REPLY_SUCCESS", "MESSAGES", keys, values));
}

/************************************************************
 * 						   	 SLOTS   						*
 ***********************************************************/

void Talk2WatchInterface::onDataReived(const QString &_data)
{
	if(m_serializer->isValid(_data))
	{
	    QHash<QString, QVariant> data = m_serializer->deserialize(_data);
	    QString category = data.value("EVENT_CATEGORY").toString();
	    QString type = data.value("EVENT_TYPE").toString();

	    data.remove("EVENT_CATEGORY");
	    data.remove("EVENT_TYPE");

	    handleMessage(type, category, data);
	}
	else
	    emit receivedData(_data);
}


void Talk2WatchInterface::onTalk2WatchLookup()
{
    bb::system::InvokeQueryTargetsReply *reply = qobject_cast<bb::system::InvokeQueryTargetsReply*>(sender());
    if (reply && reply->error() == bb::system::InvokeReplyError::None)
    {
    	QList<bb::system::InvokeAction> actions = reply->actions();

    	for(int i=0; i<actions.size(); i++)
    	{
    		QList<bb::system::InvokeTarget> targets = actions.at(i).targets();
    		for(int j=0; j<targets.size(); j++)
    		{
    			qDebug() << targets.at(j).name();
    			if(targets.at(j).name()=="com.Talk2WatchProService")
    			{
    				m_talk2WatchProServiceAvailable = true;
    				qDebug() << "Talk2WatchProService found";
    			}
    			if(targets.at(j).name()=="com.Talk2WatchPro")
    			{
    				m_talk2WatchProAvailable = true;
    				qDebug() << "Talk2WatchPro found";
    			}
    			if(targets.at(j).name()=="com.Talk2Watch.invocation.msg")
    			{
    				m_talk2WatchAvailable = true;
    				qDebug() << "Talk2Watch found";
    			}
    		}
    	}

		emit transmissionReady();

    	reply->deleteLater();
    } else if (reply && reply->error() != bb::system::InvokeReplyError::None){
    	qDebug() << "ERROR: " << reply->error();
    	reply->deleteLater();
    } else {
    	qDebug() << "reply not found";
    }


}
