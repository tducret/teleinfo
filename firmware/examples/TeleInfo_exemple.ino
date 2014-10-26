/*
   Cet exemple de code montre l'utilisation de la bibliothèque teleinfo.
   Vous devez connecter votre Spark Core à un compteur ERDF via la liaison TeleInformation.
   Plus d'infos sur le principe d'interface TeleInfo >> Arduino sur http://domotique-info.fr/2014/05/recuperer-teleinformation-arduino/
   On utilise la pin RX du Spark Core, et un optocoupleur.
*/

#include "TeleInfo/TeleInfo.h"

// ======== MAIN ============

TeleInfo ti; // création de l'objet téléinfo

// Init
char c; // pour stocker un caractère reçu sur le port série
bool trameComplete = false;

unsigned int mypApp = TELEINFO_UINT_INVALIDE;       // puissance apparente
unsigned int myiInst = TELEINFO_UINT_INVALIDE;      // courant instantané
unsigned int myindexHC = TELEINFO_UINT_INVALIDE;    // index heures creuses
unsigned int myindexHP = TELEINFO_UINT_INVALIDE;    // index heures pleines
  
void setup()
{
	Serial.begin(1200);  // port série RX/TX du Spark core
	// on permet la récupération des variables via le cloud Spark Core
	Spark.variable("papp", &mypApp, INT);
    Spark.variable("iinst", &myiInst, INT);
    Spark.variable("indexhc", &myindexHC, INT);
    Spark.variable("indexhp", &myindexHP, INT);
}

// ======== RECUPERATION SUR LE SPARK CLOUD ============
/*
Si tout se passe bien, on peut récupérer les valeurs du compteur via :

https://api.spark.io/v1/devices/<DEVICE_ID>/<VARIABLE>?access_token=<ACCESS_TOKEN>
    avec :
    <DEVICE_ID> est l’identifiant du module Spark Core : le cerveau de la carte. 
    <VARIABLE> est le nom de la variable à récupérer : papp, iinst, indexhc ou indexhp
    <ACCESS_TOKEN> est le jeton d’accès sécurisé
*/

void loop()
{
	while (Serial.available())
	{
		c = (Serial1.read() & 0x7F);  // suppression du 8ème bit (la trame téléinfo est sur 7 bits)
		trameComplete = ti.decode(c); // on envoie chaque caractère reçu dans la fonction d'encodage
		if (trameComplete) // si la trame est complète
		{
		  // on récupère les données extraites de la trame téléinfo
			mypApp=ti.pApp();
			myiInst=ti.iInst();
			myindexHC=ti.indexHC();
			myindexHP=ti.indexHP();
		}
	}
}

