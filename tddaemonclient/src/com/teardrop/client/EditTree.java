/** 
Copyright 2008 Olivier COUPELON
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.
*/

package com.teardrop.client;

import java.util.HashSet;
import java.util.Iterator;

import com.google.gwt.http.client.Request;
import com.google.gwt.http.client.RequestBuilder;
import com.google.gwt.http.client.RequestCallback;
import com.google.gwt.http.client.RequestException;
import com.google.gwt.http.client.Response;
import com.google.gwt.http.client.URL;
import com.google.gwt.json.client.JSONArray;
import com.google.gwt.json.client.JSONParser;
import com.google.gwt.json.client.JSONValue;
import com.google.gwt.user.client.Window;
import com.gwtext.client.core.Ext;
import com.gwtext.client.core.ExtElement;
import com.gwtext.client.core.SortDir;
import com.gwtext.client.core.EventObject;
import com.gwtext.client.data.BooleanFieldDef;
import com.gwtext.client.data.FieldDef;
import com.gwtext.client.data.Record;
import com.gwtext.client.data.RecordDef;
import com.gwtext.client.data.SimpleStore;
import com.gwtext.client.data.Store;
import com.gwtext.client.data.StringFieldDef;
import com.gwtext.client.widgets.Button;
import com.gwtext.client.widgets.TabPanel;
import com.gwtext.client.widgets.Toolbar;
import com.gwtext.client.widgets.ToolbarButton;
import com.gwtext.client.widgets.event.ButtonListenerAdapter;
import com.gwtext.client.widgets.form.ComboBox;
import com.gwtext.client.widgets.form.TextField;
import com.gwtext.client.widgets.grid.*;
import com.gwtext.client.widgets.grid.event.GridCellListenerAdapter;

public class EditTree {
	private static final String GET_AVAILABLE_ENGINES = "/config/get_available_engines";
	private static final String SAVE_ENGINES = "/config/save_engines";
	TabPanel centerPanel;
	EngineTree engTree;
	EditorGridPanel editPanel;
	Store engineStore;
	SimpleStore categStore;
	ComboBox categBox;
	ExtElement gridElement;
	
	private final RecordDef recordEngineDef = new RecordDef(new FieldDef[]{
			new BooleanFieldDef("select"),
			new StringFieldDef("categ"),
			new StringFieldDef("file"),  
			new StringFieldDef("name"),  
			new StringFieldDef("version")
		});
	
	private final RecordDef recordCategDef = new RecordDef(new FieldDef[]{ 
			new StringFieldDef("name")
		});
	
	public EditTree(TabPanel centerPanel, EngineTree engTree) {
		this.centerPanel = centerPanel;
		this.engTree = engTree;
		
		//Store initialisation
		engineStore = new Store(recordEngineDef);
		engineStore.setDefaultSort("name", SortDir.ASC);
		categStore = new SimpleStore(new String[]{"name"},new Object[][]{
                new Object[]{}});
		categStore.setDefaultSort("name",SortDir.ASC);
		
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
		categBox = new ComboBox();  
	    categBox.setDisplayField("name");  
		categBox.setStore(categStore);
		categColumn.setEditor(new GridEditor(categBox));
		
		ColumnConfig fileColumn = new ColumnConfig("File", "file", 600);
		TextField editFile = new TextField();
		fileColumn.setEditor(new GridEditor(editFile));
		
		ColumnConfig versionColumn = new ColumnConfig("Version", "version");
		TextField editVersion = new TextField();
		versionColumn.setEditor(new GridEditor(editVersion));
		
		ColumnConfig[] columnConfigs = {
			selectColumn,
			titleColumn,  
			fileColumn,
			categColumn,
			versionColumn
		};
		
		ColumnModel columnModel = new ColumnModel(columnConfigs);  
		columnModel.setDefaultSortable(true);
		
		//Set the toolbar
		Toolbar toolbar = new Toolbar();  
		ToolbarButton addEngineButton = new ToolbarButton("Add Engine", new ButtonListenerAdapter() {  
			public void onClick(Button button, EventObject e) {  
				String cat = "";
				if (categStore.getCount() > 0)
					cat = categStore.getAt(0).getAsString("name");
				Record newRec = recordEngineDef.createRecord(new Object[]{
						Boolean.TRUE, cat, "...", "New Engine", "1.0"});  
				editPanel.stopEditing();  
				engineStore.insert(0, newRec);  
				editPanel.startEditing(0, 0);  
			}  
		});
		addEngineButton.setCls("x-btn-text-icon add");
		addEngineButton.setTooltip("Add a new engine");
		ToolbarButton saveEngineButton = new ToolbarButton("Save", new ButtonListenerAdapter() {  
			public void onClick(Button button, EventObject e) {  
				generateOutput();
			}  
		});
		saveEngineButton.setCls("x-btn-text-icon save");
		saveEngineButton.setTooltip("Save the modifications");
		toolbar.addFill();
		toolbar.addButton(saveEngineButton);
		toolbar.addButton(addEngineButton);
		
		//Initialize the grid
		editPanel = new EditorGridPanel();
		editPanel.setTitle("Engines editor");
		editPanel.setStore(engineStore);
		editPanel.setColumnModel(columnModel);
		editPanel.setTrackMouseOver(true);
		editPanel.setSelectionModel(new RowSelectionModel());
		editPanel.setClicksToEdit(1);
		editPanel.setClosable(true);
		editPanel.setBottomToolbar(toolbar);
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
		
		//Mask the panel until data are loaded
		gridElement = Ext.get(editPanel.getId());
		gridElement.mask("Looking for available engines...");
		
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
					  generateAvailableEngineList(JSONParser.parse(response.getText()));					  
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
	
	/*
	 * POST the requested URL and data.
	 */
	private void doPostURL(String post, String url) {
	    RequestBuilder builder = new RequestBuilder(RequestBuilder.POST, URL.encode(url));
	    try {
	    	builder.setHeader("Content-Length", String.valueOf(post.length()));
	    	builder.sendRequest(post, new RequestCallback() {
				public void onError(Request request, Throwable exception) {
					Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");     
				}
				
				public void onResponseReceived(Request request, Response response) {
				  if (200 == response.getStatusCode()) {
					  generateAvailableEngineList(JSONParser.parse(response.getText()));					  
					  editPanel.getView().refresh();
					  engTree.loadEngineTree();
				  } else {
					  Window.alert("Incorrect status: " + response.getStatusText());
				  }
				}
			});
    	} catch (RequestException e) {
    		Window.alert("Couldn't connect to server (could be timeout, SOP violation, etc.)");      
    	}
	}
  
    /*
     * Instead of using something like engineStore.loadJsonData(response.getText(), false);
     * I prefer to handle manually the JSon parsing, which enables to create
     * several store at the same time.
     */
    private void generateAvailableEngineList(JSONValue jsonValue) {	    
      JSONValue engines;
      if ((engines = JSONFunctions.getJSONSet(jsonValue,"engines")) != null) {
	  JSONArray enginesArray;
	  if ((enginesArray = engines.isArray()) != null) {
		HashSet categSet = new HashSet();
        for (int i = 0; i < enginesArray.size(); ++i) {
    	  Record newRecordEngine = recordEngineDef.createRecord(new Object[]{
    			JSONFunctions.getJSONSetValue(enginesArray.get(i), "select").equals("true")?Boolean.TRUE:Boolean.FALSE,
    			JSONFunctions.getJSONSetValue(enginesArray.get(i), "categ"),
    			JSONFunctions.getJSONSetValue(enginesArray.get(i), "file"),
    			JSONFunctions.getJSONSetValue(enginesArray.get(i), "name"),
    			JSONFunctions.getJSONSetValue(enginesArray.get(i), "version")});
    	  categSet.add(JSONFunctions.getJSONSetValue(enginesArray.get(i), "categ"));
    	  engineStore.addSorted(newRecordEngine);
	    }
        Iterator catIt = categSet.iterator();
        categStore.removeAll();
        while(catIt.hasNext()) {
        	Record newRecordCateg = recordCategDef.createRecord(new Object[]{catIt.next()});
        	categStore.addSorted(newRecordCateg);
        }
	  }
    }
    gridElement.unmask();
  }
    
  void generateOutput() {
	  String post = "<config>\n";
	  for(int j = 0; j < categStore.getCount(); ++j) {
		  String engineString = ""; 
		  for(int i = 0; i < engineStore.getCount(); ++i) {
			  if (engineStore.getAt(i).getAsBoolean("select") && 
					  engineStore.getAt(i).getAsString("categ").equals(categStore.getAt(j).getAsString("name"))) {
				  engineString += "\t<engine name=\"" + engineStore.getAt(i).getAsString("name");
				  if (engineStore.getAt(i).getAsString("file").indexOf("://") != -1) {
					  engineString += "\" url=\"" + engineStore.getAt(i).getAsString("file");
				  } else {
					  engineString += "\" path=\"" + engineStore.getAt(i).getAsString("file");
				  }
				  engineString += "\" version=\"" + engineStore.getAt(i).getAsString("version");
				  engineString += "\" />\n";
			  }
		  }
		  if (!"".equals(engineString)) {
			  post += "<category name=\"" + categStore.getAt(j).getAsString("name") + "\">\n" +
			  		  engineString + "</category>\n";
		  }
	  }
	  post += "</config>";
	  doPostURL(post, SAVE_ENGINES);
	  gridElement.mask("Applying the new configuration");
  }
}
