/*==================================================================*\
|
| Programme de Manipulation d'images .ppm essentiellement de type P3
|
|
|  Auteur : LEGROS Jeremy
|
|  Numero etudiant :
|
|  Filière : L2 informatique
|
|
|  Langage de programmation : C
|
|  U.E : S2IN304
|
|  Professeur : MCF GIRARD R.
|
|  Compilateur : GCC / CC
|
|  Dernier delai : 06/11/16 23:55
|
|
|  A propos de ce programme :
|
|  .Fonctionne pour des resolutions jusqu'a 9K (si l'unite centrale
|
|   le permet) mais peut fonctionner pour des resolutions plus grandes
|
|   en changeant la valeur de la constante 'RESOLUTION'.
|
|  .Les couleurs sont codees sur 8 bits ! Amelioration possible.
|
|
| .Les variables de type simple sont toujours initialisees a zero,
|
| car je ne fais pas confiance au compilateur pour le faire a ma place
|
|
\*==================================================================*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>




// -------------------------------------------------------------------
// Longueur maximale de la chaine de caractere egale a  RESOLUTION - 1
// on reserve une case pour le caractere '\0'
// -------------------------------------------------------------------


# define RESOLUTION 5





// -------------------------------------------------------------------
// Si vaut 1 affiche le parsing du fichier de chargePnm
// -------------------------------------------------------------------

# define DEBUG 0







// -------------------------------------------------------------------
// Le type tPixel pour representer les niveaux de (rouge, vert, bleu)
// -------------------------------------------------------------------

typedef struct pixel
{
    int r;

    int v;

    int b;

} tPixel;


// -------------------------------------------------------------------
// Le type tImage pour representer une image dans un tableau de pixels
// -------------------------------------------------------------------

typedef struct image
{
  int hauteur;
  int largeur;
  int type;
  int maxval;
  tPixel** img;

} tImage;


/*------------------------------------------------------------------*\
| FONCTION viderBuffer : MCF Girard R. Université de La Réunion
|---------------------------------------------------------------------
|
| Vide le buffer d'entrée
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| buffer d'entree
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien, mais tous les caracteres du buffer d'entrée ont été récupérés
|
\*------------------------------------------------------------------*/

void viderBuffer(){
    char c;
    c=getchar();
    while(c != '\n' && c !=EOF)
        c=getchar();
}



/*------------------------------------------------------------------*\
| FONCTION debug_parsing
|---------------------------------------------------------------------
|
| Permet de visualiser le deroulement de la fonction 'chargePnm'
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| char caractere : le caractere acquis par le parsing
|
| int mode_debug : 0 pour desactiver, 1 pour activer
|
| int unget : mettre à 1 si l'on utilise un unget dans le parsing
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien sur la sortie standard, mais affichage
| caracteres recuperes sur la sortie d'erreur
|
\*------------------------------------------------------------------*/


void debug_parsing(char caractere,int mode_debug,int unget)
{

  // ---------------------------------------------------------------
  // Declaration et affectation
  // ---------------------------------------------------------------

  static int position_caractere = 1;

  static int numero_ligne = 1;

  // ---------------------------------------------------------------
  // Mode debug
  // ---------------------------------------------------------------

  if (mode_debug == 1)
  {

    fprintf(stderr,"Debug parsing caractere n°%d : %c \n",position_caractere,caractere);

   if (unget == 1)
   {
     position_caractere--;
   }

   position_caractere++;

  }

  // ---------------------------------------------------------------
  // Sortie
  // ---------------------------------------------------------------

  return;

}


/*------------------------------------------------------------------*\
| FONCTION initImage                                      EXERCICE 2.1
|---------------------------------------------------------------------
|
| Retourne une structure tImage construite en initialisant :
|
|  - le champs 'hauteur' avec le paramètre entree 'haut'
|
|  - le champs 'largeur' avec le paramètre entree 'larg'
|
|  - le champs 'type' avec la valeur 0
|
|  - le champs 'maxval' avec la valeur 0
|
|  - la quantite de memoire necessaire au champs 'img'
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
|  Un entier 'haut' , qui initialisera le champ hauteur
|
|  Un entier 'larg' , qui initialisera le champ largeur
|
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
|  tImage
|
\*------------------------------------------------------------------*/

tImage initImage (int haut, int larg)
{

  // ---------------------------------------------------------------
  // Declaration de la structure à retourner
  // ---------------------------------------------------------------

  tImage nouvelle_tImage;


  // ---------------------------------------------------------------
  // Initialisation des champs
  // ---------------------------------------------------------------

  nouvelle_tImage.hauteur= haut;

  nouvelle_tImage.largeur= larg;

  nouvelle_tImage.type= 0;

  nouvelle_tImage.maxval= 0;


  // ---------------------------------------------------------------
  // Allocation memoire du nombre de tableaux de pointeurs de pixels
  // ---------------------------------------------------------------

  nouvelle_tImage.img= (tPixel **) malloc ( haut * sizeof(tPixel*) );


  // ---------------------------------------------------------------
  // Allocation memoire pour les pixels des tableaux
  // ---------------------------------------------------------------

  for (int i= 0; i< haut ;i++)
  {
    nouvelle_tImage.img[i]= (tPixel *) malloc (larg * sizeof(tPixel));
  }

  // utilisation possible de calloc



  // ---------------------------------------------------------------
  // Sortie
  // ---------------------------------------------------------------

  return nouvelle_tImage;

}





/*==================================================================*\
|
| FONCTION copieImage                                     EXERCICE 2.2
|
|---------------------------------------------------------------------
|
| La copie est faite selon :
|
| - une allocation de memoire de la taille de la 'tImage' en parametre
|
| - une copie de tous les pixels
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| Une structure de type 'tImage' a copier
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Une structure de type 'tImage', contenant la copie
|
\*==================================================================*/

tImage copieImage (tImage im)
{
  // -----------------------------------------------------------------
  // Declaration
  // -----------------------------------------------------------------

  tImage copie_de_tImage;  // 'tImage' a retourner



  // -----------------------------------------------------------------
  // Initialisation faite dans tous les cas
  // -----------------------------------------------------------------

  copie_de_tImage.type=   im.type;

  copie_de_tImage.maxval= im.maxval;

  copie_de_tImage.img=    NULL; // Pour le cas du retour par defaut



  // -----------------------------------------------------------------
  // Le cas d'une image qui n'est pas vide de 'tPixels'
  // -----------------------------------------------------------------

  if ( im.img != NULL)
  {

    // --------------------------------------------------------------
    // Allocation memoire identique a celle du parametre en entree
    // --------------------------------------------------------------

    copie_de_tImage= initImage (im.hauteur,im.largeur);



    // --------------------------------------------------------------
    // Copie de tous les tPixels
    // --------------------------------------------------------------

    for (int i=0; i < im.hauteur ; i++)
    {
      for (int j=0; j< im.largeur ; j++)
      {
        copie_de_tImage.img[i][j]= im.img[i][j];
      }
    }
  }

  // ---------------------------------------------------------------
  // Sortie
  // ---------------------------------------------------------------

  return copie_de_tImage;
}





/*==================================================================*\
| FONCTION tImage chargePnm                               EXERCICE 2.3
|---------------------------------------------------------------------
| Renvoie une 'tImage' initialisee depuis les valeurs du fichier
| passe en parametre
|---------------------------------------------------------------------
| ENTREE
|---------------------------------------------------------------------
| Une chaine de caractere represenant le nom du fichier de caracteres
|
|
|---------------------------------------------------------------------
| SORTIE
|---------------------------------------------------------------------
| Une structure de type 'tImage'
\*==================================================================*/

tImage chargePnm (char *fichier)
{


  // -----------------------------------------------------------------
  // Variables pour les opérations principales d'entree et sortie
  // -----------------------------------------------------------------

  tImage tImage_a_retourner;

  FILE * flot;

  char c;



  // -----------------------------------------------------------------
  // Variables pour le remplissage de la matrice 'img'
  // -----------------------------------------------------------------

  int ligne;

  int colonne;



  // -----------------------------------------------------------------
  // Variables pour stocker les resulats convertis extraits du fichier
  // -----------------------------------------------------------------

  int champ_largeur;

  int champ_hauteur;

  int champ_type;

  int champ_maxval;

  int champ_couleur;

  int valeur_couleur;

  tPixel pixel_de_traitement; // Pour remplir la matrice 'img'


  // -----------------------------------------------------------------
  // Variables pour recuperer les informations du fichier
  // -----------------------------------------------------------------

  char chaine_dimension[RESOLUTION];

  char chaine_couleur[4];

  int indice;



  // -----------------------------------------------------------------
  // Variables explicites pour les tests au lieu de chiffres
  // -----------------------------------------------------------------

  int rouge;
  int vert;
  int bleu;



  // -----------------------------------------------------------------
  // Initialisation tImage par defaut
  // -----------------------------------------------------------------

  tImage_a_retourner.hauteur= 0;
  tImage_a_retourner.largeur= 0;
  tImage_a_retourner.type   = 0;
  tImage_a_retourner.img    = NULL;



  // -----------------------------------------------------------------
  // Initialisation des variables de tests
  // -----------------------------------------------------------------

  rouge= 1;
  vert = 2;
  bleu = 3;



  // -----------------------------------------------------------------
  // Initialisation a zero des entiers
  // -----------------------------------------------------------------

  ligne   = 0;
  colonne = 0;
  champ_largeur =  0;
  champ_hauteur =  0;
  champ_type    =  0;
  champ_maxval  =  0;
  champ_couleur =  0;
  valeur_couleur=  0;

  indice= 0;



  // -----------------------------------------------------------------
  //  Ouverture du fichier texte en lecture
  // -----------------------------------------------------------------

  flot= fopen (fichier, "r");


  //---------------------------------------------------------------
  // Gestion de l'erreur a l'ouverture du fichier
  // ---------------------------------------------------------------

  if ( flot == NULL )
  {

    fprintf (stderr,"\nErreur:fonction chargePnm : fichier inconnu ");

    return tImage_a_retourner;

  }


  // =================================================================
  // Traitement du fichier en lecture sans erreur
  // =================================================================

  // -----------------------------------------------------------------
  //  Première ligne
  // type de l'image sur 2 caractères de la forme Px
  // -----------------------------------------------------------------

  c = fgetc(flot);// premier caractere

  debug_parsing(c,DEBUG,0);


  // -----------------------------------------------------------------
  // Deuxième caractere : la valeur du type
  // -----------------------------------------------------------------

  c = fgetc(flot);

  debug_parsing(c,DEBUG,0);


  // -----------------------------------------------------------------
  // Conversion du caractere en une chaine de char puis en un entier
  // -----------------------------------------------------------------

  chaine_couleur[0]= c;     // Utilisation de chaine_couleur evite
  chaine_couleur[1]= '\0';  // la creation d'une autre variable

  champ_type= atoi(chaine_couleur);



  // -----------------------------------------------------------------
  // ** On passe la fin de la 1ere ligne et on se positionne au début
  // de la ligne suivante
  // -----------------------------------------------------------------

  while( (c = fgetc(flot)) != '\n' )
  {
    debug_parsing(c,DEBUG,0);
  };


  // -----------------------------------------------------------------
  // On passe toutes les lignes commencant par #
  // -----------------------------------------------------------------

  while ( (c= fgetc(flot) ) =='#' )
  {

    debug_parsing(c,DEBUG,0);


    while( ( c = fgetc(flot) ) != '\n')
    {
      debug_parsing(c,DEBUG,0);
    };
  }



  // -----------------------------------------------------------------
  // Comme on a lu un caractère de trop on le remet dans le flot.
  // -----------------------------------------------------------------

  ungetc(c, flot);

  debug_parsing(c,DEBUG,1);


  // -----------------------------------------------------------------
  // ** On récupère les dimensions de l'image, la valeur maximale des // niveaux de couleur
  // -----------------------------------------------------------------


  // -----------------------------------------------------------------
  // Recuperation de la largeur
  // -----------------------------------------------------------------

  while( ( c= fgetc(flot) ) != ' ' )
  {
    chaine_dimension[indice]= c;

    debug_parsing(c,DEBUG,0);

    indice++;


    // ---------------------------------------------------------------
    // Test du nombre de chiffres de la largeur
    // ---------------------------------------------------------------

    if (indice >= RESOLUTION)
    {

     fprintf(stderr,"Erreur:fonction chargePnm: largeur trop haute");

     exit(EXIT_FAILURE);

    }
  }


  // -----------------------------------------------------------------
  // Conversion chaine de caractere vers entier 'champ_largeur'
  // -----------------------------------------------------------------

  chaine_dimension[indice]= '\0' ;

  champ_largeur= atoi(chaine_dimension);

  if(DEBUG)
  {
    fprintf(stderr,"Debug largeur:%d \n",champ_largeur);
  }

  // -----------------------------------------------------------------
  // Recuperation de la hauteur
  // -----------------------------------------------------------------

  indice= 0;

  while( ( c= fgetc(flot) ) != '\n' )
  {
    chaine_dimension[indice]= c;

    debug_parsing(c,DEBUG,0);

    indice++;

    // ---------------------------------------------------------------
    // Test du nombre de chiffres de la hauteur
    // ---------------------------------------------------------------

    if (indice >= RESOLUTION)
    {

     fprintf(stderr,"Erreur:fonction chargePnm: hauteur trop haute");

     exit(EXIT_FAILURE);

    }
  }

  // -----------------------------------------------------------------
  // Conversion chaine de caractere vers entier 'champ_hauteur'
  // -----------------------------------------------------------------

  chaine_dimension[indice]= '\0' ;

  champ_hauteur= atoi(chaine_dimension);

 if(DEBUG)
 {
   fprintf(stderr,"Debug hauteur:%d \n",champ_hauteur);
 }




  // -----------------------------------------------------------------
  // Recuperation de maxval
  // -----------------------------------------------------------------

  indice= 0;

  while( (c = fgetc(flot)) != '\n')
  {
      chaine_couleur[indice++]= c;

      debug_parsing(c,DEBUG,0);

  }

  // -----------------------------------------------------------------
  // Conversion chaine de caractere vers entier 'champ_maxval'
  // -----------------------------------------------------------------

  chaine_couleur[indice]= '\0';

  champ_maxval= atoi(chaine_couleur);

  if(DEBUG)
  {
    fprintf(stderr,"Debug maxval:%d\n",champ_maxval);
  }



  // -----------------------------------------------------------------
  // Test de l'entier 'champ_maxval'
  // -----------------------------------------------------------------

  if (champ_maxval <0 || champ_maxval > 255)
  {
      fprintf(stderr,"Erreur:fonction chargePnm: couleur maximum est  255");

      exit(EXIT_FAILURE);

  }


  // -----------------------------------------------------------------
  // Initialisation des champs de l'image que l'on veut retourner
  // -----------------------------------------------------------------


  tImage_a_retourner         = initImage(champ_hauteur,champ_largeur);
  tImage_a_retourner.hauteur = champ_hauteur;
  tImage_a_retourner.largeur = champ_largeur;
  tImage_a_retourner.type    = champ_type;
  tImage_a_retourner.maxval  = champ_maxval;


  // =================================================================
  // Remplissage de la matrice 'tImage_a_retourner.img'
  // =================================================================


  // ---------------------------------------------------------------
  // Parcours de la matrice de pixels
  // ---------------------------------------------------------------

  for (ligne= 0; ligne < champ_hauteur; ligne ++)
  {

    for (colonne = 0; colonne < champ_largeur; colonne++)
    {

  // ---------------------------------------------------------------
  // Recuperation des trois couleurs d'un pixel
  // ---------------------------------------------------------------

      for (champ_couleur= rouge; champ_couleur != 4; champ_couleur++)
      {

        indice= 0;

        while( (c = fgetc(flot)) != '\n')
        {

            chaine_couleur[indice++]= c;

            debug_parsing(c,DEBUG,0);


        }

        chaine_couleur[indice]='\0';


        // -----------------------------------------------------------
        // Conversion de la chaine de caracteres vers une couleur
        // -----------------------------------------------------------

        valeur_couleur= atoi(chaine_couleur);

        if(DEBUG)
        {
          fprintf(stderr,"Debug:valeur_couleur:%d \n",valeur_couleur);
        }


        // -----------------------------------------------------------
        // Verification de la valeur de la couleur
        // -----------------------------------------------------------

        if ( valeur_couleur < 0 || valeur_couleur > champ_maxval)
        {
          fprintf(stderr,"Erreur:fonction chargePnm: maxval");

          exit(EXIT_FAILURE);
        }

        // -----------------------------------------------------------
        // Initialisation du 'pixel_de_traitement'
        // -----------------------------------------------------------

        if ( champ_couleur == rouge )
        {
          pixel_de_traitement.r= valeur_couleur;
        }

        if ( champ_couleur == vert )
        {
          pixel_de_traitement.v= valeur_couleur;
        }

        if ( champ_couleur == bleu )
        {
          pixel_de_traitement.b= valeur_couleur;
        }


      }

        tImage_a_retourner.img[ligne][colonne]= pixel_de_traitement;
    }
  }


  // ----------------------------------------
  // Sortie :
  // ----------------------------------------

      fclose(flot);


      return tImage_a_retourner;

}





/*------------------------------------------------------------------*\
| FONCTION void sauvePnm                                  EXERCICE 2.4
|---------------------------------------------------------------------
|
| Ecrit le contenu de 'im' donne en parametre dans un fichier
|
| 'nom.pnm'
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| char * nom : chaine de caractere
|
| tImage im : tImage a transcrire
|
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien, mais un fichier Pnm construit selon 'im' a ete cree
|
\*------------------------------------------------------------------*/

void sauvePnm(char * nom,tImage im)
{

  // -----------------------------------------------------------------
  // Declaration
  // -----------------------------------------------------------------

  int ligne;

  int colonne;

  int hauteur;

  int largeur;

  FILE *flux;

  char * auteur= "# Auteur: LEGROS Jeremy";

  char * extension= ".ppm";

  char nom_de_fichier[80];

  strcat(nom_de_fichier,nom);

  strcat(nom_de_fichier,extension);

  // -----------------------------------------------------------------
  // Initialisation
  // -----------------------------------------------------------------

  ligne  = 0;
  colonne= 0;
  hauteur= im.hauteur;
  largeur= im.largeur;

  // -----------------------------------------------------------------
  // Ouverture du fichier
  // -----------------------------------------------------------------

  flux=fopen(nom_de_fichier,"w");

  //---------------------------------------------------------------
  // Gestion de l'erreur de fichier inconnu
  // ---------------------------------------------------------------

  if ( flux == NULL )
  {

    fprintf (stderr,"\nErreur:fonction sauvePnm : fichier inconnu ");

    return;

  }

  // -----------------------------------------------------------------
  // Ecriture du type
  // -----------------------------------------------------------------

  fprintf(flux,"P%d\n",im.type);

  // -----------------------------------------------------------------
  // Ecriture de l'auteur
  // -----------------------------------------------------------------

  fprintf(flux,"%s\n",auteur);

  // -----------------------------------------------------------------
  // Ecriture de la resolution
  // -----------------------------------------------------------------

  fprintf(flux,"%d %d\n",largeur,hauteur);


  for (ligne = 0;ligne < hauteur; ligne ++ )
  {
    for (colonne = 0; colonne < largeur; colonne ++)
    {

      //--------------------------------------------------------------
      // Ecriture des 3 valeurs de couleur pour chaque pixel
      //--------------------------------------------------------------

      fprintf(flux,"%d\n",im.img[ligne][colonne].r);

      fprintf(flux,"%d\n",im.img[ligne][colonne].v );

      fprintf(flux,"%d\n",im.img[ligne][colonne].b );

    }
  }

  // -----------------------------------------------------------------
  // Fermeture du fichier
  // -----------------------------------------------------------------

  fclose(flux);


  return;
}




/*------------------------------------------------------------------*\
| FONCTION miroir                                         EXERCICE 2.5
|---------------------------------------------------------------------
|
| Effectue un effet miroir sur les pixels
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage *im : le lien vers le fichier 'im' a modifier
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien, mais la matrice 'im.img' est modifiee avec un effet miroir
|
\*------------------------------------------------------------------*/

void miroir(tImage *im)
{

  // -----------------------------------------------------------------
  // Declaration
  // -----------------------------------------------------------------

  int indice_milieu;

  int indice_partant_de_la_fin;

  int indice_partant_du_debut;

  int dimension_paire;

  tPixel pixel_pour_echange;

  int ieme_ligne;        // iterateur pour parcourir toutes les lignes



  // -----------------------------------------------------------------
  // Initialisation
  // -----------------------------------------------------------------

  dimension_paire= ((*im).largeur % 2  == 0 );

  indice_partant_de_la_fin= ( (*im).largeur - 1 );

  indice_partant_du_debut= 0;

  indice_milieu= ((*im).largeur / 2) + 1;  // pas d'echange au milieu

  ieme_ligne= 0;


 // -----------------------------------------------------------------
 // Si la dimension est paire
 // -----------------------------------------------------------------
 // type : [ pixel ] [ pixel ] [ pixel ] [ pixel ]

 if (dimension_paire)
 {

  // -----------------------------------------------------------------
  //
  // Pour chaque ligne
  //
  // -----------------------------------------------------------------

  for (ieme_ligne= 0; ieme_ligne < (*im).hauteur; ieme_ligne++)
  {

    // --------------------------------------------------------------
    // Condition d'arret : meme indice
    // --------------------------------------------------------------

    if (indice_partant_du_debut == indice_partant_de_la_fin )
    {
      break;
    }



    // --------------------------------------------------------------
    // On echange le contenu des cases qui sont à l'oppose l'une de
    // l'autre
    // --------------------------------------------------------------

    pixel_pour_echange= (*im).img[ieme_ligne][indice_partant_du_debut];

    (*im).img[ieme_ligne][indice_partant_du_debut]= (*im).img[ieme_ligne][indice_partant_de_la_fin];

    (*im).img[ieme_ligne][indice_partant_de_la_fin]= pixel_pour_echange;



    // --------------------------------------------------------------
    // On parcourt la ligne dans les deux sens
    // --------------------------------------------------------------

    indice_partant_du_debut++;

    indice_partant_de_la_fin--;

  }
 }

  //----------------------------------------------------------------
  // Si la dimension est impaire
  //----------------------------------------------------------------
  // type : [ pixel ] [ pixel ] [ pixel ] [ pixel ] [ pixel ]


  for (ieme_ligne= 0; ieme_ligne < (*im).hauteur; ieme_ligne++)
  {

    // --------------------------------------------------------------
    // Condition d'arret : on ne traite pas le pixel du milieu
    // --------------------------------------------------------------

    if (indice_partant_du_debut != indice_milieu)
    {
      break;
    }



    // --------------------------------------------------------------
    // On echange le contenu des cases qui sont à l'oppose l'une de
    // l'autre
    // --------------------------------------------------------------

    pixel_pour_echange= (*im).img[ieme_ligne][indice_partant_du_debut];

    (*im).img[ieme_ligne][indice_partant_du_debut]= (*im).img[ieme_ligne][indice_partant_de_la_fin];

    (*im).img[ieme_ligne][indice_partant_de_la_fin]= pixel_pour_echange;



    // ---------------------------------------------------------------
    // On parcourt la ligne dans les deux sens
    // ---------------------------------------------------------------

    indice_partant_du_debut++;

    indice_partant_de_la_fin--;

  }

  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------

  return;

}



/*------------------------------------------------------------------*\
| FONCTION modifContraste                                 EXERCICE 2.6
|---------------------------------------------------------------------
|
| Modifie le contraste de l'image donne en parametre avec la valeur
|
| donnee en parametre.
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage *im : le lien vers l'image à modifier
|
| float k : la valeur du contraste
|
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien, mais le contraste de 'im' a ete modifie.
|
|
\*------------------------------------------------------------------*/

void modifContraste(tImage *im, float k)
{
  //------------------------------------------------------------------
  // Declaration
  //------------------------------------------------------------------

  int ligne;
  int colonne;
  int hauteur;
  int largeur;
  tPixel pixel_de_traitement;



  //------------------------------------------------------------------
  // Initialisation
  //------------------------------------------------------------------

  ligne  = 0;
  colonne= 0;

  hauteur=(*im).hauteur;
  largeur=(*im).largeur;

  //------------------------------------------------------------------
  // Gestion des cas d'erreur
  //------------------------------------------------------------------


  if (k < 0)
  {
    fprintf(stderr,"fonction:modifContraste: k < 0 ");

    exit(EXIT_FAILURE);
  }



  if (k == 0)
  {
    fprintf(stderr,"fonction:modifContraste: k == 0 ");

    exit(EXIT_FAILURE);
  }

  //------------------------------------------------------------------
  // Traitement de tous les pixels
  //------------------------------------------------------------------


 for (ligne = 0;ligne < hauteur; ligne++)
 {
   for (colonne = 0;colonne < largeur; colonne++)
   {

     pixel_de_traitement    = (*im).img[ligne][colonne];

     // --------------------------------------------------------------
     // Traitement du contraste du pixel
     // --------------------------------------------------------------

     pixel_de_traitement.r *= k;

     pixel_de_traitement.v *= k;

     pixel_de_traitement.b *= k;



     // --------------------------------------------------------------
     // Traitement du depassement de la valeur de couleur maximum
     // --------------------------------------------------------------

     if (pixel_de_traitement.r > (*im).maxval)
     {
       pixel_de_traitement.r = (*im).maxval;
     }

     if (pixel_de_traitement.v > (*im).maxval)
     {
       pixel_de_traitement.v = (*im).maxval;
     }

     if (pixel_de_traitement.b > (*im).maxval )
     {
       pixel_de_traitement.b = (*im).maxval;
     }

     // --------------------------------------------------------------
     // Application de la modification
     // --------------------------------------------------------------

    (*im).img[ligne][colonne] = pixel_de_traitement;

   }
 }

  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------

  return;
}



/*------------------------------------------------------------------*\
| FONCTION modifBrillance                                 EXERCICE 2.7
|---------------------------------------------------------------------
|
| Modifie la brillance de la 'tImage' en entree depuis le parametre
| en entree
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage *im : lien vers image a modifier
|
| int b : valeur de la brillance a appliquer
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
|
\*------------------------------------------------------------------*/

void modifBrillance(tImage *im, int b)
{

  //------------------------------------------------------------------
  // Declaration
  //------------------------------------------------------------------

  int ligne;
  int colonne;
  int hauteur;
  int largeur;
  tPixel pixel_de_traitement;



  //------------------------------------------------------------------
  // Initialisation
  //------------------------------------------------------------------

  ligne  = 0;
  colonne= 0;

  hauteur=(*im).hauteur;
  largeur=(*im).largeur;



  //------------------------------------------------------------------
  // Traitement de tous les pixels
  //------------------------------------------------------------------


 for (ligne = 0;ligne < hauteur; ligne++)
 {
   for (colonne = 0;colonne < largeur; colonne++)
   {

     pixel_de_traitement    = (*im).img[ligne][colonne];

     // --------------------------------------------------------------
     // Traitement de la brillance du pixel
     // --------------------------------------------------------------

     pixel_de_traitement.r += b;

     pixel_de_traitement.v += b;

     pixel_de_traitement.b += b;



     // --------------------------------------------------------------
     // Traitement d'une valeur negative
     // --------------------------------------------------------------

     if (pixel_de_traitement.r < 0 )
     {
       pixel_de_traitement.r *= -1;
     }

     if (pixel_de_traitement.v < (*im).maxval)
     {
       pixel_de_traitement.v *= -1;
     }

     if (pixel_de_traitement.b < 0 )
     {
       pixel_de_traitement.b *= -1;
     }

     // --------------------------------------------------------------
     // Application de la modification
     // --------------------------------------------------------------

    (*im).img[ligne][colonne] = pixel_de_traitement;

   }
 }

  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------

  return;
}





/*------------------------------------------------------------------*\
| FONCTION luminance                                      EXERCICE 2.8
|---------------------------------------------------------------------
|
| Calcule la luminance du pixel donne en parametre
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tPixel p : pixel dont on doit calculer la luminence
|
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| float : la luminance calculee
|
\*------------------------------------------------------------------*/

float luminance(tPixel p)
{
  return 0.2125 * p.r + 0.7154 * p.v + 0.0721 * p.b;
}




/*------------------------------------------------------------------*\
| FONCTION niveauGris                                     EXERCICE 2.9
|---------------------------------------------------------------------
|
| Transforme l'image donne en parametre en niveau de gris
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage *im : lien vers la tImage a modifier
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| Rien mais 'im' est en niveaux de gris
|
\*------------------------------------------------------------------*/

void niveauGris(tImage *im)
{

  //------------------------------------------------------------------
  // Declaration & Initialisation
  //------------------------------------------------------------------

  float luminance_pixel = 0;

  //------------------------------------------------------------------
  // Traitement de tous les pixels
  //------------------------------------------------------------------

  for (int i= 0; i < (*im).hauteur ; i++)
  {
    for (int j= 0; j < (*im).largeur ; j++ )
    {

      // -------------------------------------------------------------
      // Application de luminance
      // -------------------------------------------------------------

      luminance_pixel= luminance( (*im).img[i][j] );

      (*im).img[i][j].r= luminance_pixel;
      (*im).img[i][j].v= luminance_pixel;
      (*im).img[i][j].b= luminance_pixel;

    }
  }

  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------

  return;
}


/*------------------------------------------------------------------*\
| FONCTION reduction                                     EXERCICE 2.10
|---------------------------------------------------------------------
|
| Modifie l'image donnee en parametre en divisant ses dimensions par | deux
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage im : image a modifier
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| tImage : copie de 'im' avec dimensions
|
\*------------------------------------------------------------------*/

tImage reduction(tImage im)
{

  //------------------------------------------------------------------
  // Declaration
  //------------------------------------------------------------------

  int hauteur;
  int largeur;
  int moitie_hauteur;
  int moitie_largeur;

  int moitie_hauteur_est_paire;
  int moitie_largeur_est_paire;


  int ligne;
  int colonne;

  tImage tImage_a_retourner;


  //------------------------------------------------------------------
  // Initialisation
  //------------------------------------------------------------------

  ligne  = 0;
  colonne= 0;

  moitie_hauteur_est_paire= 0;
  moitie_largeur_est_paire= 0;



  hauteur= im.hauteur;
  largeur= im.largeur;


  moitie_hauteur= hauteur / 2;
  moitie_largeur= largeur / 2;




  // =================================================================
  // Gestion du cas de dimension impaire
  // =================================================================


  moitie_hauteur_est_paire= ( hauteur %2 == 0 );

  moitie_largeur_est_paire= ( largeur %2 == 0 );


  //------------------------------------------------------------------
  // moitie hauteur impaire
  //------------------------------------------------------------------


  if ( !moitie_hauteur_est_paire )
  {
      moitie_hauteur += 1;
  }



  //------------------------------------------------------------------
  // moitie largeur impaire
  //------------------------------------------------------------------

  if ( !moitie_largeur_est_paire )
  {
      moitie_largeur += 1;
  }


  tImage_a_retourner       = initImage(moitie_hauteur,moitie_largeur);

  tImage_a_retourner.type  = im.type;

  tImage_a_retourner.maxval= im.maxval;

  // ================================================================
  // Remplissage de la matrice de pixels de l'image a retourner
  // ================================================================


  for(ligne= 0; ligne < hauteur ;ligne ++)
  {
    for(colonne= 0; colonne < largeur ;colonne ++ )
    {
      if ( ligne % 2 == 0) // une ligne sur deux : ligne paire
      {
        if( (colonne %2 == 0)) // une colonne sur deux : colonne paire
        {
         tImage_a_retourner.img[ligne][colonne]=im.img[ligne][colonne];
        }
      }
    }
  }

  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------

  return tImage_a_retourner;
}





/*------------------------------------------------------------------*\
| FONCTION fois2bilineaire  NON FAIT             EXERCICE 2.11
|---------------------------------------------------------------------
|
| Renvoie une image avec les dimensions de l'image d'entree doublees
|
|---------------------------------------------------------------------
| ENTREE :
|---------------------------------------------------------------------
|
| tImage im
|
|---------------------------------------------------------------------
| SORTIE :
|---------------------------------------------------------------------
|
| tImage : copie de 'im' avec dimensions doublees
|
\*------------------------------------------------------------------*/

tImage fois2bilineaire(tImage im)
{
  //------------------------------------------------------------------
  // Declaration
  //------------------------------------------------------------------

  int hauteur;
  int largeur;
  int moitie_hauteur;
  int moitie_largeur;

  int moitie_hauteur_est_paire;
  int moitie_largeur_est_paire;


  int ligne;
  int colonne;

  tImage tImage_a_retourner;


  //------------------------------------------------------------------
  // Initialisation
  //------------------------------------------------------------------

  ligne  = 0;
  colonne= 0;

  moitie_hauteur_est_paire= 0;
  moitie_largeur_est_paire= 0;



  hauteur= im.hauteur;
  largeur= im.largeur;


  moitie_hauteur= hauteur / 2;
  moitie_largeur= largeur / 2;




  // =================================================================
  // Gestion du cas de dimension impaire
  // =================================================================


  moitie_hauteur_est_paire= ( hauteur %2 == 0 );

  moitie_largeur_est_paire= ( largeur %2 == 0 );


  //------------------------------------------------------------------
  // moitie hauteur impaire
  //------------------------------------------------------------------


  if ( !moitie_hauteur_est_paire )
  {
      moitie_hauteur += 1;
  }



  //------------------------------------------------------------------
  // moitie largeur impaire
  //------------------------------------------------------------------

  if ( !moitie_largeur_est_paire )
  {
      moitie_largeur += 1;
  }


  tImage_a_retourner       = initImage(moitie_hauteur,moitie_largeur);

  tImage_a_retourner.type  = im.type;

  tImage_a_retourner.maxval= im.maxval;

  // ================================================================
  // Remplissage de la matrice de pixels de l'image a retourner
  // ================================================================


  //------------------------------------------------------------------
  // Sortie
  //------------------------------------------------------------------


  return tImage_a_retourner;
}








// ===================================================================
// Programme principal                                   EXERCICE 2.12
// ===================================================================

int main(int nombre_arguments, char *ligne_de_commande[])
{

 // ------------------------------------------------------------------
 // Declaration
 // ------------------------------------------------------------------

  int indice;

  int numero_operation;

  int valeur_scanf;

  int valeur_parametre;

  char caractere;

  char nom_de_fichier_origine[80];

  char nom_de_fichier_copie[80];

  tImage tImage_a_travailler;

  tImage copie_image;

  tImage * lien_copie_image;


 // ------------------------------------------------------------------
 // Initialisation
 // ------------------------------------------------------------------

  indice= 0;

  numero_operation= 0;

  valeur_scanf= 0;

  valeur_parametre= 0;



 // ------------------------------------------------------------------
 // Debut du programme : demande du nom du fichier
 // ------------------------------------------------------------------


  printf("Quel est le nom du fichier a traiter ?\n");

  printf("**Attention ni de caracteres spéciaux ni espaces**");

  printf("\n");

 // ------------------------------------------------------------------
 // Recuperation du nom du fichier
 // ------------------------------------------------------------------

  indice= 0;

  while( (caractere= getchar()) != '\n')
  {
     nom_de_fichier_origine[indice]= caractere;
     indice++;
  }

  nom_de_fichier_origine[indice]= '\0';


  printf("vous avez choisi de travailler sur %s \n",nom_de_fichier_origine);


  tImage_a_travailler= chargePnm(nom_de_fichier_origine);


 // ------------------------------------------------------------------
 // Demande du nom du fichier pour la copie
 // ------------------------------------------------------------------


  printf("Quel est le nom du fichier pour la copie ?\n");

  printf("**Attention ni de caracteres spéciaux ni espaces**");

  printf("\n");

  printf("Entrer un nom de fichier inferieur à 50 caracteres \n");


 // ------------------------------------------------------------------
 // Recuperation du nom du fichier pour la copie
 // ------------------------------------------------------------------

  indice= 0;

  while( (caractere= getchar()) != '\n')
  {
     nom_de_fichier_copie[indice]= caractere;
     indice++;
  }

  nom_de_fichier_copie[indice]= '\0';


  printf("vous avez choisi le nom %s pour la copie \n",nom_de_fichier_copie);


 // ------------------------------------------------------------------
 // Copie de la structure tImage
 // ------------------------------------------------------------------


  tImage tImage_copie;


  tImage_copie= copieImage(tImage_a_travailler);


  lien_copie_image= &tImage_copie;



 // ------------------------------------------------------------------
 // Menu des operations possibles
 // ------------------------------------------------------------------

 printf("---------------------------------------------------------");
 printf("\n");


 printf("Choisissez un nombre correspondant à l'opération voulue \n");

 printf("---------------------------------------------------------");
 printf("\n");


 printf("0 : Quitter sans sauvegarder\n");

 printf("1 : Miroir \n");
 printf(">Transforme l’image un effet miroir sur les pixels\n");

 printf("2 : Contraste\n");
 printf("> Modifie le contraste par un parametre \n");


 printf("3 : Brillance\n");
 printf("> Modifie la brillance par un parametre \n");

 printf("4 : Niveaux de gris\n");
 printf("> Transforme une image couleur en niveaux de gris \n");


 printf("5 : Reduction de l'image par deux\n");
 printf("> Transforme une image couleur en niveaux de gris \n");


 printf("6 : agrandissement de l'image\n");
 printf("> Agrandit une image d'un rapport de 2 \n");


 valeur_scanf= scanf("%d",&numero_operation);


 if (valeur_scanf != 1)
 {
   fprintf(stderr,"Erreur:valeur_scanf : %d",valeur_scanf);

   exit(EXIT_FAILURE);
 }

 viderBuffer();

 // ------------------------------------------------------------------
 // Menu des operations possibles
 // ------------------------------------------------------------------


 switch(numero_operation)
 {

    case 0 :

      exit(EXIT_SUCCESS);

      break;

   case 1 :

     printf("======================================================");
     printf("\n");

     printf("Transformation miroir");

     printf("\n");
     printf("======================================================");
     printf("\n");

     miroir(lien_copie_image);

     printf("Modification de la brillance terminee\n");

     break;

   case 2 :

     printf("======================================================");
     printf("\n");

     printf("Modification du contraste");

     printf("\n");
     printf("======================================================");
     printf("\n");

     printf("Pour diminuer le contraste ");
     printf("choisir une valeur positive tel que : 0 < valeur < 1\n");

     printf("Pour augmenter le contraste ");
     printf("choisir une valeur tel que valeur > 1\n");

     printf("Veuillez saisir une valeur pour le contraste\n");


    // ---------------------------------------------------------------
    // Entree du parametre du contraste
    // ---------------------------------------------------------------

     valeur_scanf= scanf("%d",&valeur_parametre);

     if (valeur_scanf != 1)
     {
       fprintf(stderr,"Erreur:main:scanf:contraste %d",valeur_scanf);

       exit(EXIT_FAILURE);
     }

     viderBuffer();

    // ---------------------------------------------------------------
    // Traitement du contraste de l'image
    // ---------------------------------------------------------------

     modifContraste(lien_copie_image,valeur_parametre);

     break;

   case 3 :

     printf("======================================================");
     printf("\n");

     printf("Modification de la brillance");
     printf("\n");

     printf("======================================================");
     printf("\n");


     printf("Pour diminuer la brillance ");
     printf("choisir une valeur negative \n");

     printf("Pour augmenter la brillance ");
     printf("choisir une valeur tel que valeur positive\n");

     printf("Veuillez saisir une valeur pour la brillance\n");


    // ---------------------------------------------------------------
    // Entree du parametre de la brillance
    // ---------------------------------------------------------------


     valeur_scanf= scanf("%d",&valeur_parametre);

     if (valeur_scanf != 1)
     {
       fprintf(stderr,"Erreur:main:scanf:brillance: %d",valeur_scanf);

       exit(EXIT_FAILURE);
     }

     viderBuffer();


    // ---------------------------------------------------------------
    // Traitement de la brillance de l'image
    // ---------------------------------------------------------------

     modifBrillance(lien_copie_image,valeur_parametre);

     printf("Modification de la brillance terminee\n");

     break;

   case 4 :

     niveauGris(lien_copie_image);
     printf("Conversion en niveau de gris terminee\n");

     break;

   case 5 :

     reduction(copie_image);
     printf("Reduction terminee\n");
     break;

   case 6 :

     fois2bilineaire(copie_image);
     printf("Agrandissiment terminee\n");
     break;

   default:

     printf("Valeur inconnue, sortie du programme\n");

 }


 // ------------------------------------------------------------------
 // Sauvegarde du fichier et sortie
 // ------------------------------------------------------------------

 sauvePnm(nom_de_fichier_copie,copie_image);


 printf("Sauvegarde reussie, sortie \n");


 return 0;

}
