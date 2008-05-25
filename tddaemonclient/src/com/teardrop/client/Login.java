/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.Window;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.event.KeyListener;
import com.gwtext.client.widgets.form.FormPanel;
import com.gwtext.client.widgets.form.Label;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.layout.AnchorLayoutData;
import com.gwtext.client.widgets.layout.FitLayout;

public class Login {
	private static final String LOGIN_URL = "/login";
	private LoginCallback lc;
	private Window loginWindow;
	
	public interface LoginCallback {
		public void onCloseRun();
	}

	public Login(LoginCallback lc) {
		this.lc = lc;
		initializeLoginForm();
	}

	private void initializeLoginForm() {
		
		final FormPanel authentPanel = new FormPanel();
		authentPanel.setFrame(false);
		authentPanel.setHeader(false);
		authentPanel.setBaseCls("x-plain");
		authentPanel.setWidth(350);
		authentPanel.setHeight(200);
		
		Label label = new Label();  
		label.setHtml("<p>Please provide your login information for connecting Teardrop.</p><br />");
		label.setHeight(20);
		authentPanel.add(label, new AnchorLayoutData("100%"));
	
		TextField loginText = new TextField("Login", "username");
		loginText.setAllowBlank(false);
		loginText.addKeyListener(EventObject.ENTER, new KeyListener() {
			public void onKey(int key, EventObject e) {
				if (authentPanel.getForm().isValid())
					doAuthenticate(authentPanel.getForm().getValues());
			}});
		authentPanel.add(loginText, new AnchorLayoutData("90%"));
		
		TextField passwdText = new TextField("Password","password");
		passwdText.setPassword(true);
		passwdText.setAllowBlank(false);
		passwdText.addKeyListener(EventObject.ENTER, new KeyListener() {
			public void onKey(int key, EventObject e) {
				if (authentPanel.getForm().isValid())
					doAuthenticate(authentPanel.getForm().getValues());
			}});
		authentPanel.add(passwdText, new AnchorLayoutData("90%"));
		
		Button authentButton = new Button("Sign In", new ButtonListenerAdapter() {  
			public void onClick(Button button, EventObject e) {
				if (authentPanel.getForm().isValid())
					doAuthenticate(authentPanel.getForm().getValues());
			}  
		});
		authentButton.setType("submit");
		authentPanel.addButton(authentButton);
			
		loginWindow = new Window("Teardrop Login", true, true);
		loginWindow.setClosable(false);
		loginWindow.setWidth(350);
		loginWindow.setHeight(200);
		loginWindow.setMinWidth(250);
		loginWindow.setMinHeight(175);
		loginWindow.setPaddings(10);
		loginWindow.setLayout(new FitLayout());
		loginWindow.add(authentPanel);
		loginWindow.show();
	}
	
	void doAuthenticate(String post) {
		RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(LOGIN_URL));
	    try {
	    	builder.setHeader("Content-Length", String.valueOf(post.length()));
	    	builder.sendRequest(post, new RequestCallback() {
	    	    public void onError(Request request, Throwable exception) {
	    	    	// ("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
	    	    }

	    	    public void onResponseReceived(Request request, Response response) {
	    	    	lc.onCloseRun();
	    	    	loginWindow.close();
	    	    }
	    	  });
	    	} catch (RequestException e) {
	    		//setProgessMessage("Couldn't connect to server (could be timeout, SOP violation, etc.)");      
	    	}
	}
	
}
