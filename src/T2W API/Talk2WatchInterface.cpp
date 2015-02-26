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

Talk2WatchInterface::Talk2WatchInterface(QObject *_parent)
{

	m_udp = new UdpModule(this);
	m_udp->listenOnPort(8484);
	connect(m_udp, SIGNAL(reveivedData(QString)), this, SLOT(onDataReived(QString)));

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
	QString command = "APP_AUTH_RQ$$";
	if(m_protocol=="TCP")
		command += m_appName + "$$" + m_appVersion + "$$" + m_appKey + "$$TCP$$" + m_port + "$$" + m_description;
	else if(m_protocol=="UDP")
		command += m_appName + "$$" + m_appVersion + "$$" + m_appKey + "$$UDP$$" + m_port + "$$" + m_description;

	sendCommand(command);
}

/*	RECEIVING MESSAGES - STEP 3
 *
 *	Use the creator methods to create the actions you want to add to Talk2Watch.
 */

void Talk2WatchInterface::createAction(QString _title, QString _command, QString _description)
{
	QString command = "APP_CREATE_ACTION$$" + _title + "$$" + m_appKey + "$$" + m_appName + "$$" + _command + "$$" + _description;
	sendCommand(command);
}

void Talk2WatchInterface::createAction(QString _title, QString _folder, QString _command, QString _description)
{
    QString command = "APP_CREATE_ACTION$$" + _title + "$$" + _folder + "$$" + m_appKey + "$$" + m_appName + "$$" + _command + "$$" + _description;
    sendCommand(command);
}

void Talk2WatchInterface::createFolder(QString _title)
{
    createFolder(_title, "root");
}

void Talk2WatchInterface::createFolder(QString _title, QString _parentFolder)
{
    QString command = "APP_CREATE_FOLDER$$" + _title + "$$" + _parentFolder + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::removeConnection()
{
    QString command = "APP_REMOVE_CONNECTION$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::removeAction(const QString &_action)
{
    QString command = "APP_REMOVE_ACTION$$" + _action + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::removeFolder(const QString &_folder)
{
    QString command = "APP_REMOVE_FOLDER$$" + _folder + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::renameAction(const QString &_oldTitle, const QString &_newTitle)
{
    QString command = "APP_RENAME_ACTION$$" + _oldTitle + "$$" + _newTitle + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::renameFolder(const QString &_oldTitle, const QString &_newTitle)
{
    QString command = "APP_RENAME_FOLDER$$" + _oldTitle + "$$" + _newTitle + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
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
    QString command = "APP_REGISTER_UUID$$" + _uuid + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
}

void Talk2WatchInterface::deregisterAppMessageListener(const QString &_uuid)
{
    QString command = "APP_DEREGISTER_UUID$$" + _uuid + "$$" + m_appName + "$$" + m_appKey;
    sendCommand(command);
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

void Talk2WatchInterface::handleMessage(const QString &_type, const QString &_category, const QHash<QString, QVariant> &_values)
{
    if(_category=="PEBBLE")
    {
        qDebug() << _type << _values;

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
}

/************************************************************
 * 						   	 SLOTS   						*
 ***********************************************************/

void Talk2WatchInterface::onDataReived(const QString &_data)
{
    qDebug() << "_VALID_?" <<  m_serializer->isValid(_data);

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
	qDebug() << "onTalk2WatchProAvailableResponse()";
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
