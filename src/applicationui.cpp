/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include "Talk2WatchInterface.h"
#include "UdpModule.h"
#include <bb/system/SystemToast>

using namespace bb::cascades;

#include "PebbleNotifications/pebblenotification.h"

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{

    /* IMPORTANT
     *
     * Change the port and do not use 8484
     * Set it to -1 if you only want to send messages
     *
     */

    t2w = new Talk2WatchInterface(8484, this);


    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("t2w", t2w);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    app->setScene(root);

    m_authStatusLabel = root->findChild<Label*>("m_authStatusLabel");

    connect(t2w, SIGNAL(transmissionReady()), this, SLOT(onTransmissionReady()));
}


void ApplicationUI::onTransmissionReady()
{
	qDebug() << "onTransmissionReady";

	/*  Uncommend the following lines for an example of Pebble notifications.
	 *  The app will send a notification on startup.
	 */

	/*


    // change this to your own app values (and don't use port 8484!)
    // the method has to be called before a notification is sent
	t2w->setAppValues("NewApp", "0.1", "appKey", "UDP", "8484", "");

	connect(t2w, SIGNAL(dismissReceived(QString)), this, SLOT(onDismissReceived(QString)));
    connect(t2w, SIGNAL(genericReceived(QString, QString)), this, SLOT(onGenericReceived(QString, QString)));
    connect(t2w, SIGNAL(responseReceived(QString, QString)), this, SLOT(onResponseReceived(QString, QString)));

    PebbleNotification notification("Title", "Subtitle", "Body");
    notification.addDismissAction("Dismiss");
    notification.addResponseAction("Reply", QStringList() << "text1" << "text2" << "text3");
    notification.addGenericAction("Action");
    t2w->sendPebbleNotification(notification);

    */

}

void ApplicationUI::onDismissReceived(const QString &_id)
{
    // handle the dismiss event


    // no acknowledgement
}
void ApplicationUI::onGenericReceived(const QString &_id, const QString &_text)
{
    // handle the generic event


    // send an acknowledgement
    t2w->sendPebbleNotificationAcknowledgment(_id, "Success");
}
void ApplicationUI::onResponseReceived(const QString &_id, const QString &_text)
{
    // handle the response event


    // send an acknowledgement
    t2w->sendPebbleNotificationAcknowledgment(_id, "Transmitted");
}
