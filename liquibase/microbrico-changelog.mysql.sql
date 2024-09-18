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

-- changeset adrie:1716202953457-12 comment:Add column to kit ot detect if it is a game or not
ALTER TABLE kit ADD COLUMN `isgame` TINYINT NULL DEFAULT 0 AFTER `caution`;

-- changeset adrie:1716202953457-13 comment:increase field "nom" size from 60 to 150 in KIT table
ALTER TABLE kit CHANGE COLUMN `nom` `nom` VARCHAR(150) NULL DEFAULT NULL ;

-- changeset adrie:1716202953457-14 comment:increase field "name" size from 60 to 100 in item table
ALTER TABLE item CHANGE COLUMN `name` `name` VARCHAR(100) NULL DEFAULT NULL ;

-- changeset adrie:1716202953457-15 comment:create sortie table
CREATE TABLE `db_dev`.`sortie` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `start_date` DATE NOT NULL,
  `id_user` INT NULL,
  `id_kit` INT NULL,
  `id_sortie` INT NULL,
  PRIMARY KEY (`id`));
  
-- changeset adrie:1716202953457-16 comment:create uid_sortie table
  CREATE TABLE `db_dev`.`uid_sortie` (
  `iduid_sortie` INT NOT NULL,
  PRIMARY KEY (`iduid_sortie`),
  UNIQUE INDEX `iduid_sortie_UNIQUE` (`iduid_sortie` ASC) VISIBLE);


-- changeset adrie:1716202953457-17 comment:set utinfo in utilisateur table to "unique" attribute
ALTER TABLE `db_dev`.`utilisateur` 
ADD UNIQUE INDEX `utinfo_UNIQUE` (`utinfo` ASC) VISIBLE;
;

-- changeset adrie:1716202953457-18 comment:set utinfo to char 10
ALTER TABLE `db_dev`.`utilisateur` 
CHANGE COLUMN `utinfo` `utinfo` CHAR(10) NOT NULL ;

-- changeset adrie:1716202953457-19 comment:add quantity_init column to item
ALTER TABLE `db_dev`.`item` 
ADD COLUMN `quantity_init` SMALLINT NOT NULL AFTER `quantity`;

-- changeset adrie:1716202953457-20 comment:add is_active column to resa table
ALTER TABLE `db_dev`.`resa` 
ADD COLUMN `is_active` TINYINT NULL DEFAULT 1 AFTER `id_resa`;

-- changeset adrie:1716202953457-21 comment:add is_active column to sortie table
ALTER TABLE `db_dev`.`sortie` 
ADD COLUMN `is_active` TINYINT NULL DEFAULT 1 AFTER `id_sortie`;