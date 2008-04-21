#include "UIDSession.h"

UIDSession::UIDSession(){
}

UIDSession::~UIDSession(){
}

string UIDSession::getID() {
	char *alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string id ="";
  for(int index=0; index<29; index++){
  	char c = alphanum[int(rand()/(RAND_MAX + 1.0)*62)];
    id += c; 
  }
  return id;
}

void UIDSession::initRand() {
	srand((unsigned)time(0));
}
