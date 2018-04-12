/**
 * @brief noeud de l'arbre
 * 
 * Noeud de l'arbre
 */
typedef struct sNode
{
	char* command; /** chaine de caractère contenant la commande*/
	bool success; /** boolean indiquant le succés ou l'echec de la sous commande*/
	char* result; /** chaine de caractère contenant le résultat de la commande */
	char* separator; /** chaine de caractère contenant le séparateur */
	char* inputValue; /** chaine de caractère contenant le la valeur d'entré de la commande */
	struct sNode* leftChild; /** fils droit*/
	struct sNode* rightChild;	/** fils gauche*/
} Node;