#include "namecalc.h"
#include <cctype>

int getCharAlphaPos(char c)
{
    if (!std::isalpha(static_cast<unsigned char>(c)))
        return 0;

    c = std::tolower(static_cast<unsigned char>(c));
    return (c - 'a' + 1);
}

int getAlphanum(std::string input)
{
    int sum = 0;
    for (char c : input)
        sum += getCharAlphaPos(c);
    return sum;
}

int getNbVoyelles(std::string input)
{
    int count = 0;
    for (char c : input)
    {
        c = std::tolower(static_cast<unsigned char>(c));
        if (c=='a' || c=='e' || c=='i' || c=='o' || c=='u' || c=='y')
            count++;
    }
    return count;
}

int getNbConsonnes(std::string input)
{
    int count = 0;
    for (char c : input)
    {
        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            c = std::tolower(static_cast<unsigned char>(c));
            if (!(c=='a' || c=='e' || c=='i' || c=='o' || c=='u' || c=='y'))
                count++;
        }
    }
    return count;
}

int getNbLettres(std::string input)
{
    int count = 0;
    for (char c : input)
        if (std::isalpha(static_cast<unsigned char>(c)))
            count++;
    return count;
}

double calcCoeff(std::string prenom, std::string nom)
{
    int alpha = getAlphanum(prenom) + getAlphanum(nom);
    int lettres = getNbLettres(prenom) + getNbLettres(nom);

    if (lettres == 0)
        return 0.0;

    return static_cast<double>(alpha) / lettres;
}

void printInfos(std::string prenom, std::string nom)
{
    std::cout << "Prenom : " << prenom << std::endl;
    std::cout << "Nom    : " << nom << std::endl;

    std::cout << "Lettres        : "
              << getNbLettres(prenom) + getNbLettres(nom) << std::endl;
    std::cout << "Voyelles       : "
              << getNbVoyelles(prenom) + getNbVoyelles(nom) << std::endl;
    std::cout << "Consonnes      : "
              << getNbConsonnes(prenom) + getNbConsonnes(nom) << std::endl;
    std::cout << "Alphanumerique : "
              << getAlphanum(prenom) + getAlphanum(nom) << std::endl;
    std::cout << "Coefficient    : "
              << calcCoeff(prenom, nom) << std::endl;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Usage: ./namecalc Prenom Nom" << std::endl;
		return 1;
	}
	printInfos(argv[1], argv[2]);
	return 0;
}

