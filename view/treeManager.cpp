#include "treeManager.h"
#include "icons/xmag.xpm"
#include "icons/folder_html.xpm"

treeManager::treeManager(Ui::MainWindow *ui) {
	this->ui = ui;
  	generateModel();
}

treeManager::~treeManager() {
	//TODO: Remove every QTreeWidgetItem created 
	delete treeModel;
}

void treeManager::generateModel() {

	ui->engineTreeView->setHeaderLabel("Engines");
	ui->engineTreeView->setColumnCount(2);
	ui->engineTreeView->setColumnHidden(1,true);
	
	xmlFile xf;
	string config_path;
	
	if (selectFile::find("config",".xml",config_path) && xf.openFile(config_path + "config.xml")) {
		QIcon icCateg = QIcon(QPixmap(folder_html_xpm));
		QList<QTreeWidgetItem *> items;
		
		nodeDoc ndCateg(&xf,"category");
		while(ndCateg.isValid()) {
			QTreeWidgetItem *category = new QTreeWidgetItem(QStringList(ndCateg.getAttributeValueByName("name").c_str()));
			category->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			category->setCheckState(0,Qt::Unchecked);
			category->setIcon(0,icCateg);
			items.append(category);
			nodeDoc ndEngine(&xf,"engine",ndCateg);
			while(ndEngine.isValid()) {
				string filename = ndEngine.getAttributeValueByName("name");
				string path;
				QIcon *ic;

				if (selectFile::find(filename,".png",path)) {
					ic = new QIcon(string(path + filename + ".png").c_str());
				} else {
					ic = new QIcon(QPixmap(xmag_xpm));
				}
				
				xmlEngine xe;
				if (selectFile::find(filename,".xml",path) && xe.openEngine(filename)) {
					QTreeWidgetItem *engine = new QTreeWidgetItem(QStringList(xe.getName().c_str()));
					engine->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
					engine->setCheckState(0,Qt::Unchecked);
					engine->setIcon(0,*ic);
					engine->setText(1,filename.c_str());
					category->addChild(engine);
				} else {
					// The file we a looking for was not found.
					cerr << "The file " << filename << ".xml was not found." << endl;
				}
				delete ic;
				ndEngine.next();
			}
			ndCateg.next();
		}
		ui->engineTreeView->insertTopLevelItems(0, items);
		ui->engineTreeView->sortItems(0,Qt::AscendingOrder);
	} else {
        cerr << "Teardrop configuration file could not be found." << endl;
	}
}

void treeManager::engineItemChanged(QTreeWidgetItem *it) {
	//cout << "The item " << it->text(0).toStdString() << " was changed on column " << c << endl;
	//If we are on a category node :
	if (it->child(0) != NULL) {
		//Sets all the categories' engines to the same check state 
		for(int i = 0; it->child(i) != NULL; ++i) {
			it->child(i)->setCheckState(0,it->checkState(0));
		}
	}
}

void treeManager::getListEngines(list<string> & eng, int c) {
	for(int i = 0; ui->engineTreeView->topLevelItem(i) != NULL; ++i) {
		QTreeWidgetItem *it = ui->engineTreeView->topLevelItem(i); 
		for(int j = 0; it->child(j) != NULL; ++j) {
			if (it->child(j)->checkState(0)) {
				//Add engine to eng;
				eng.push_back(it->child(j)->data(c,Qt::DisplayRole).toString().toStdString());
			}
		}
	}
}
