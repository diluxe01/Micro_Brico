Prérequis pour travailler sur le projet: 
    - Installer QT Creator 12.0.2 (avec QT 6.6.1, je ne sais plus si QTCreator installe QT 6.6.1 lors de son installation)
    - Installer mysql 8.0.35 https://dev.mysql.com/downloads/installer/
    - Installer Mysql workbench
    - Installer github desktop
    
Pour lancer le server Mysql(service windows): 
	Il existe un service windows. Le retrouver et le lancer.
	service name: Mysql@localhost:3306
	Pour lancer un service dans un invite de cmd: 
	sc start mysql80
	sc stop mysql80
	ce service ne permet pas par défaut d'autoriser des connections autres que localement. 
	pour autoriser des connections externes, il faut ajouter la ligne bind_address=0.0.0.0 dans le fichier C:\ProgramData\MySQL\MySQL Server 8.0\my.ini

Pour configurer une première connection:
	ALTER USER 'root'@'localhost' IDENTIFIED BY 'MOT_DE_PASSE'; --(change mdp root)
	create user 'adrien'@'localhost' identified by 'adrien'; --(créer un utilisteur)
	grant all on dbtest.* to 'adrien'@'localhost'; --(Donne tous les privilèges à l'utilisateur)
    
    
Pour se connecter au server:
    utiliser Mysql Workbench, en tant que "root" sur le port localhost:3306
    
Installation de QT 
    Pour que QTCreator fonctionne avec mysql il faut obtenir les bonnes DLL:

    Il faut récupérer le driver qt correspondant à la version de QT présent sur la machine et au compilateur présent sur le PC (mingw probablement)
    Le lien suivant propose de télécharger le driver https://github.com/thecodemonkey86/qt_mysql_driver/releases
    Une fois le dossier zippé téléchargé, déziper, puis: 
        copier coller les 3 DLL libcrypto-3-x64, libmysql, libssl-3-x64 dans le dossier C:\Qt\QT\6.6.1\mingw_64\bin
        copier coller les libs contenues dans le dossier sqldrivers ici: C:\Qt\QT\6.6.1\mingw_64\plugins\sqldrivers
