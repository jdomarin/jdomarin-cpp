/*
 * File : namecalc.h
 * 
 * Copyright 2012 Jérémy Domarin <jeremy@jeremy-laptop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

/*
 * Calcule l'alphanumérisation d'une chaîne de caractères (ici nom ou prénom)
 * name: getAlphanum
 * @param input la chaîne de caractères dont on veut obtenir l'alphanumérisation
 * @return la somme des positions dans l'alphabet latin des lettres du prénom et du nom
 */
int getAlphanum(std::string input);

/*
 * Obtient le nombre de consonnes dans une chaîne de caractères
 * name: getNbConsonnes
 * @param input la chaîne de caractères dont on veut obtenir le nombre de consonnes
 * @return le nombre de consonnes de la chaîne d'entrée
 */
int getNbConsonnes(std::string input);

/*
 * Obtient le nombre de voyelles dans une chaîne de caractères
 * name: getNbVoyelles
 * @param input la chaîne de caractères dont on veut obtenir le nombre de voyelles
 * @return le nombre de voyelles de la chaîne d'entrée
 */
int getNbVoyelles(std::string input);

/*
 * Obtient le nombre de lettres (consonnes et voyelles) dans une chaîne de caractères
 * name: getNbLettres
 * @param input la chaîne de caractères dont on veut obtenir le nombre de lettres
 * @return un entier représentant le nombre de lettres contenus dans le nom et le prénom d'une personne
 */
int getNbLettres(std::string input);

/*
 * Obtient la position d'une lettre donnée dans l'alphabet latin
 * name: getCharAlphaPos
 * @param c un caractère (lettre de l'alphabet)
 * @return un entier représentant la position dans l'alphabet latin du caractère c
 */
int getCharAlphaPos(char c);

/*
 * Calcule un coefficient représentant une métrique pertinente pour une identité (nom et prénom)
 * name: calcCoeff
 * @param prenom chaîne représentant le prénom de la personne saisie en entrée standard
 * @param nom chaîne représentant le nom de la personne saisie en entrée standard
 * @return un nombre décimal représentant le quotient entre l'alphanumérisation et le nombre de lettres dans une identité (prénom, nom)
 */
double calcCoeff(std::string prenom, std::string nom);

/*
 * Affiche sur la sortie standard (cout), les informations numériques
 * concernant le prénom et le nom saisis sur l'entrée standard
 * name: printInfos
 * @param nom le nom de la personne dont on veut avoir les informations
 * @param prenom le prénom de la personne dont on veut avoir les informations
 * @return
 */
void printInfos(std::string prenom, std::string nom);

