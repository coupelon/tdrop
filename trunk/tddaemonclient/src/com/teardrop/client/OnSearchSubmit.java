/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import com.gwtext.client.core.EventObject;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.CycleButton;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.event.KeyListener;

public class OnSearchSubmit extends ButtonListenerAdapter implements KeyListener {
	EngineTree engTree;
	TextField queryText;
	CycleButton limitButton;
	TabPanel centerPanel;
	public OnSearchSubmit(EngineTree engTree, TextField queryText, CycleButton limitButton, TabPanel centerPanel) {
		this.engTree = engTree;
		this.queryText = queryText;
		this.limitButton = limitButton;
		this.centerPanel = centerPanel;
	}
	
	public void onClick(Button button, EventObject e) {
		submit();
    }
	
	public void onKey(int key,EventObject e) {
		submit();
	}
	
	public void submit() {
		new PerformSearch(engTree, queryText,limitButton,centerPanel);
	}
}
