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
import com.google.gwt.user.client.Window;
import com.gwtext.client.core.SortDir;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.data.BooleanFieldDef;
import com.gwtext.client.data.FieldDef;
import com.gwtext.client.data.JsonReader;
import com.gwtext.client.data.Record;
import com.gwtext.client.data.RecordDef;
import com.gwtext.client.data.Store;
import com.gwtext.client.data.StringFieldDef;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.grid.*;
import com.gwtext.client.widgets.grid.event.GridCellListenerAdapter;

public class EditTree {
	private static final String GET_AVAILABLE_ENGINES = "/config/get_available_engines";
	TabPanel centerPanel;
	EditorGridPanel editPanel;
	Store engineStore;
	
	private final RecordDef recordEngineDef = new RecordDef(new FieldDef[]{
			new BooleanFieldDef("select"),
			new StringFieldDef("categ"),
			new StringFieldDef("file"),  
			new StringFieldDef("name"),  
			new StringFieldDef("version")
		});
	
	public EditTree(TabPanel centerPanel) {
		this.centerPanel = centerPanel;
		
		//Initialise the columns model
		ColumnConfig selectColumn = new ColumnConfig("Selected", "select");
		selectColumn.setRenderer(new Renderer() {  
			public String render(Object value, CellMetadata cellMetadata, Record record, int rowIndex, int colNum, Store store) {  
				boolean checked = ((Boolean) value).booleanValue();  
				return "<img class=\"checkbox\" src=\"js/ext/resources/images/default/menu/" +  
				(checked ? "checked.gif" : "unchecked.gif") + "\"/>";  
				}  
		});
		
		ColumnConfig titleColumn = new ColumnConfig("Name", "name");  
		titleColumn.setCss("white-space:normal;");
		titleColumn.setSortable(true);
		TextField editTitle = new TextField();
		titleColumn.setEditor(new GridEditor(editTitle));
		
		ColumnConfig categColumn = new ColumnConfig("Category", "categ");
		
		ColumnConfig fileColumn = new ColumnConfig("File", "file", 600);
		ColumnConfig versionColumn = new ColumnConfig("Version", "version");
		
		ColumnConfig[] columnConfigs = {
			selectColumn,
			titleColumn,  
			fileColumn,
			categColumn,
			versionColumn
		};
		
		ColumnModel columnModel = new ColumnModel(columnConfigs);  
		columnModel.setDefaultSortable(true);
		
		//Json store initialisation
		JsonReader engineReader = new JsonReader(recordEngineDef);
		engineReader.setRoot("engines");
		engineStore = new Store(engineReader);
		engineStore.setDefaultSort("name", SortDir.ASC);
		
		//Initialize the grid
		editPanel = new EditorGridPanel();
		editPanel.setTitle("Engines editor");
		editPanel.setStore(engineStore);
		editPanel.setColumnModel(columnModel);
		editPanel.setTrackMouseOver(true);
		editPanel.setSelectionModel(new RowSelectionModel());
		editPanel.getView().setForceFit(true);
		editPanel.getView().setAutoFill(true);
		editPanel.setClicksToEdit(1);
		editPanel.setClosable(true);
		editPanel.addGridCellListener(new GridCellListenerAdapter() {
            public void onCellClick(GridPanel grid, int rowIndex, int colIndex, EventObject e) {
                if (grid.getColumnModel().getDataIndex(colIndex).equals("select") && e.getTarget(".checkbox", 1) != null) {
                    Record record = grid.getStore().getAt(rowIndex);
                    record.set("select", !record.getAsBoolean("select"));
                }
            }
        });
		
		centerPanel.add(editPanel);
		centerPanel.activate(centerPanel.getItems().length-1);
		
		doFetchURL(GET_AVAILABLE_ENGINES);
	}
	
	/*
	 * Fetch the requested URL.
	 */
	private void doFetchURL(String url) {
	    RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(url));
	    try {
	    	builder.sendRequest("", new RequestCallback() {
				public void onError(Request request, Throwable exception) {
					Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
				}
				
				public void onResponseReceived(Request request, Response response) {
				  if (200 == response.getStatusCode()) {
					  engineStore.loadJsonData(response.getText(), false);
					  editPanel.getView().refresh();
				  } else {
					  Window.alert("Incorrect status: " + response.getStatusText());
				  }
				}
			});
    	} catch (RequestException e) {
    		Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");      
    	}
	}
}
