-- liquibase formatted sql

-- changeset adrie:1716202953457-1
CREATE TABLE item (id INT AUTO_INCREMENT NOT NULL, name VARCHAR(45) NULL, forkey INT NULL, quantity SMALLINT NOT NULL, CONSTRAINT PK_ITEM PRIMARY KEY (id), UNIQUE (id));

-- changeset adrie:1716202953457-2
CREATE TABLE kit (idkit INT AUTO_INCREMENT NOT NULL, nom VARCHAR(60) NULL, `description` MEDIUMTEXT NULL, date_achat date NULL, prix_achat DECIMAL(9, 2) NULL, texte_libre MEDIUMTEXT NULL, en_panne TINYINT(3) UNSIGNED NULL, code_kit VARCHAR(45) NOT NULL, caution DECIMAL(9, 2) UNSIGNED NULL, CONSTRAINT PK_KIT PRIMARY KEY (idkit), UNIQUE (code_kit));

-- changeset adrie:1716202953457-3
CREATE TABLE resa (id INT AUTO_INCREMENT NOT NULL, start_date date NULL, id_user INT NULL, id_kit INT NULL, id_resa INT NULL, CONSTRAINT PK_RESA PRIMARY KEY (id));

-- changeset adrie:1716202953457-4
CREATE TABLE uid_resa (iduid_resa INT UNSIGNED NOT NULL, CONSTRAINT PK_UID_RESA PRIMARY KEY (iduid_resa));

-- changeset adrie:1716202953457-5
CREATE TABLE utilisateur (id INT AUTO_INCREMENT NOT NULL, nom VARCHAR(45) NOT NULL, mdp CHAR(40) NOT NULL, prenom VARCHAR(45) NOT NULL, email VARCHAR(45) NOT NULL, token CHAR(40) NULL, utinfo CHAR(6) NOT NULL, privilege CHAR(10) DEFAULT 'basic' NOT NULL, CONSTRAINT PK_UTILISATEUR PRIMARY KEY (id));

-- changeset adrie:1716202953457-6
CREATE INDEX forkey_id_kit_idx ON resa(id_kit);

-- changeset adrie:1716202953457-7
CREATE INDEX forkey_id_user_idx ON resa(id_user);

-- changeset adrie:1716202953457-8
CREATE INDEX forkey_idx ON item(forkey);

-- changeset adrie:1716202953457-9
ALTER TABLE item ADD CONSTRAINT forkey FOREIGN KEY (forkey) REFERENCES kit (idkit) ON UPDATE CASCADE ON DELETE CASCADE;

-- changeset adrie:1716202953457-10
ALTER TABLE resa ADD CONSTRAINT forkey_id_kit FOREIGN KEY (id_kit) REFERENCES kit (idkit) ON UPDATE CASCADE ON DELETE CASCADE;

-- changeset adrie:1716202953457-11
ALTER TABLE resa ADD CONSTRAINT forkey_id_user FOREIGN KEY (id_user) REFERENCES utilisateur (id) ON UPDATE CASCADE ON DELETE SET NULL;
