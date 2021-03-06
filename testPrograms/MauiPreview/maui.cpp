/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// connect this application to a maui-comm server.

#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Util.h>
#include <MAUtil/Map.h>
#include <MAUI/Engine.h>
#include <conprint.h>

#include "MAHeaders.h"

using namespace MAUtil;
using namespace MAUI;

enum State {
	STATE_COMMAND,
	STATE_RESOURCES
};

class MyScreen : public Screen, ConnectionListener {
public:
    MyScreen() : mConnection(this), state(STATE_COMMAND) {
        int res = mConnection.connect("socket://localhost:8080");
        if(res < 0) {
            maPanic(res, "mConnection.connect failed");
        }
		resourcePlaceholder = maCreatePlaceholder();
    }

    ~MyScreen() {
        // todo: delete main widget of this screen
    }

    void getData() {
		int offset = buffer.size();
		buffer.reserve(offset+1024);
        mConnection.recv(&buffer[offset], 1024);
    }

    void writeResponse(const char *c, int len) {
    	mConnection.write(c, len);
    }

    void connectFinished(Connection* conn, int result) {
         if(result < 0) {
             printf("mConnection.connectFinished failed");
             return;
         }
         getData();
     }

    void connWriteFinished(Connection* conn, int result) {
        if(result < 0) {
            printf("mConnection.write failed");
            return;
        }
        getData();
    }

    Widget *getWidget(const String& id) {
    	Widget **w= widgets.get(id);
    	if(w==NULL) maPanic(0, "Widget id doesn't exist.");
    	return *w;
    }

    void printWidgetIds() {
		printf("printing keys:");
		const Vector<String>& keys = widgets.getKeySet();
		for(int i = 0; i < keys.size(); i++)  {
			printf("'%s'", keys[i].c_str());
		}
    }

	void recieveResources() {
		state = STATE_RESOURCES;
		resourceDataCreated = false;
		currentResourceSize = 0;
		getData();
	}

	bool loadResources() {
		if(resourceDataCreated == false) {
			if(buffer.size() < 4) return false;
			resourceSize =
				((int)buffer[0])|
				(((int)buffer[1])<<8)|
				(((int)buffer[2])<<16)|
				(((int)buffer[3])<<24);

			maCreateData(resourcePlaceholder, resourceSize);
			maWriteData(resourcePlaceholder, &buffer[4], currentResourceSize, buffer.size());
			currentResourceSize+=buffer.size()-4;
		} else {
			maWriteData(resourcePlaceholder, &buffer[0], currentResourceSize, buffer.size());
			currentResourceSize+=buffer.size();
		}

		if(currentResourceSize!=resourceSize) return false;
		else {
			maLoadResources(resourcePlaceholder);
			maDestroyObject(resourcePlaceholder);
			return true;
		}
	}

    void executeCommands(const Vector<String>& commands) {
    	if(commands.size() < 1) return;

    	if(commands[0] == "addWidget") {
    		if(commands.size() != 3) return; // addWidget type id
    		if(commands[1] == "Label") {
    			printf("added label with");
    			printf("id: '%s'", commands[2].c_str());
    			widgets[commands[2]] = new Label(0, 0, 0, 0, 0);
    			printWidgetIds();
    		}
    	} else
     	if(commands[0] == "setParam") {
    		if(commands.size() != 4) return; // setParam id name value
   			printf("setting param of widget with id");
    		printf("id: '%s'", commands[1].c_str());

    		printWidgetIds();
    		getWidget(commands[1])->setParameter(commands[2], commands[3]);
     	} else
     	if(commands[0] == "addChild") {
    		if(commands.size() != 3) return; // addChild idParent idChild
    		getWidget(commands[1])->add(widgets[commands[2]]);
     	} else
        if(commands[0] == "setRoot") {
        	if(commands.size() != 2) return; // setRoot id
			setMain(getWidget(commands[1]));
			show();
        } else
        if(commands[0] == "loadResources") {
			writeResponse("done", 4);
			recieveResources();
		}
    }

    bool parseCommands() {
    	Vector<String> commands;
		buffer.resize(buffer.size()+1);
    	buffer[buffer.size()-1] = 0;
		buffer.resize(buffer.size()-1);

    	int i = 0;
    	String s((char*)&buffer[0]);
    	if((i=s.find(";")) == String::npos) return false;
    	s[i] = 0;
    	printf("%s", s.c_str());
    	stringSplit(s, " ", commands);

       	for(int i = 0; i < commands.size(); i++) {
       		printf("'%s'", commands[i].c_str());
			commands[i].resize(strlen(commands[i].c_str()));
       	}

       	executeCommands(commands);

       	return true;
    }

    void connRecvFinished(Connection* conn, int result) {
        if(result < 0) {
            printf("mConnection.recv failed");
            return;
        }

		buffer.resize(buffer.size()+result);

		int done = 0;

		if(state == STATE_COMMAND) {
			if(parseCommands()) done = 1;

		} else if(state == STATE_RESOURCES) {
			if(loadResources()) done = 1;
		}

		if(done) {
			writeResponse("done", 4);
			buffer.clear();
		} else {
			getData();
		}

    }

private:
	Vector<byte> buffer;
	Connection mConnection;

	Map<String, Widget*> widgets;

	State state;
	Handle resourcePlaceholder;
	bool resourceDataCreated;
	int currentResourceSize;
	int resourceSize;

};

class MAUIMoblet : public Moblet {
public:
    MAUIMoblet() {
        // todo: initializtion
    	Engine::getSingleton().setDefaultFont(new Font(FONT0));
        screen = new MyScreen();
        screen->show();
    }

    void keyPressEvent(int keyCode) {
        // todo: handle key presses
    }

    void keyReleaseEvent(int keyCode) {
        // todo: handle key releases
    }
    MyScreen* screen;

    ~MAUIMoblet() {
        delete screen;
    }

};

extern "C" int MAMain() {
	gConsoleDisplay = 0;
    Moblet::run(new MAUIMoblet());
    return 0;
};
