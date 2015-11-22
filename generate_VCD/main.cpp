#include <iostream>

#include "settings.h"
#include "uart_gestion.h"


#define BUFF_SIZE   256

using namespace std;

int main()
{
    Settings* set_data = new Settings();
    UART_gestion* serial_com = new UART_gestion();

    //Déclaration d'un buffer qui reçoit les données
    char buffer[BUFF_SIZE];
    int nb_bytes_read;

    //lecture du fichier de config et récupération des données
    set_data->getSettings("init.txt");

    //configuration de la strucutre concernant la com série en fonction des données du fichier de config
    serial_com->SetDcbStructure(set_data->getBaud(), set_data->getNbBits(), set_data->getBitsStop(), set_data->getParity());

    //ouverture du port com en fonction du numéro donné dans le fichier de config
    cout << "tentative d'ouverture du COM" << set_data->getComNumber() << endl;
    if( serial_com->OpenCOM(set_data->getComNumber())){
        //la communication série est ouverture
        cout << "COM Serie ouverte et prete a l'emploi" << endl;
       //opération de lecture sur la com série
        while(1){
            serial_com->ReadCOM(&buffer, 1, &nb_bytes_read);

        }
        //fermeture du port com
        cout << "fermeture de la communication" << endl;
        serial_com->CloseCOM();
    }


    delete set_data;
    delete serial_com;
    return 0;
}

