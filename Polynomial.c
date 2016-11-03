#include "Polynomial.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initializes *P as described below.
 *
 * Pre:  P points to an uninitialized Polynomial object,
 *       C != NULL,
 *       C[i] initialized for i = 0:D
 * Post: P->Degree == D,
 *       P->Coeff != C (array is duplicated, not linked),
 *       P->Coeff[i] == C[i] for i = 0:D
 * Returns: false if *P cannot be properly initialized, true otherwise
 */
 
bool Polynomial_Set(Polynomial* const P, const uint8_t D, const int64_t* const C)
{
   if (D > 0 && D < 255)
   {
       P->Degree = D;
	   if (P->Coeff != NULL)
	   {
	      free(P->Coeff);
	   }
	   P->Coeff = malloc((D + 1) * sizeof(int64_t));
	   for (int i = 0; i < D; i++)
       {
           P->Coeff[i] = C[i];
       }
	   return true;
   }
   
   return false;
}

/**
 * Initializes *Target from *Source as described below.
 *
 * Pre:  Target points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object
 * Post: Target->Degree == Source->Degree,
 *       Target->Coeff != Source->Coeff,
 *       Target->Coeff[i] == Source->Coeff[i] for i = 0:Source->Degree
 * Returns: false if *Target cannot be properly initialized, true otherwise
 */
bool Polynomial_Copy(Polynomial* const Target, const Polynomial* const Source)
{
    return Polynomial_Set(Target, Source->Degree, Source->Coeff);
}

/**
 * Compares two polynomials.
 *
 * Pre:  Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object
 * Returns: true if Left and Right have the same coefficients, false otherwise
 */
bool Polynomial_Equals(const Polynomial* const Left, const Polynomial* const Right)
{
    if (Left->Coeff == Right->Coeff)
	{
	    return 0;
	}
	return 1;
}

/**
 * Computes value of polynomial at X.
 *
 * Pre:  P points to a properly-initialized Polynomial object
 * Returns: value of P(X); 0 if cannot be evaluated
 */
int64_t Polynomial_EvaluateAt(const Polynomial* const P, const int64_t X)
{
    double sum = 0.0;
	double power = 1.0;
    for ( int i = 0; i < P->Degree; i++)
	{
	    sum += power * (P->Coeff[i]);
		power *= X;
	}
	return sum;
}

/**
 * Initializes *Scaled to represent K times *Source
 *
 * Pre:  Scaled points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object,
 *       Source != Target
 * Post: Scaled->Degree == Source->Degree,
 *       Scaled->Coeff  != Source->Coeff,
 *       Scaled->Coeff[i] == K * Source->Coeff[i] for i = 0:Scaled->Degree
 * Returns: false if *Scaled cannot be properly initialized, true otherwise
 */
bool    Polynomial_Scale(Polynomial* const Scaled, const Polynomial* const Source, const int64_t K)
{
    Scaled->Degree = Source->Degree;
    if (Source->Coeff != NULL)
	{
	    free(Scaled->Coeff);
	}
	Scaled->Coeff = malloc((Source->Degree + 1) * sizeof(int64_t));
	if (Scaled->Coeff == NULL) 
	{
	    return false;
	}
	for (int i = 0; i < Source->Degree; i++)
    {
        Scaled->Coeff[i] = K * Source->Coeff[i];
    }
	return true;	
}

/**
 * Initializes *Sum to equal *Left + *Right.
 *
 * Pre:  Sum points to a Polynomial object,
 *       Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object,
 *       Sum != Left,
 *       Sum != Right
 * Post: Sum->Degree == max(Left->Degree, Right->Degree),
 *       Sum->Coeff[i] == Left->Coeff[i] + Right->Coeff[i] 
 *           with proper allowance for the case that 
 *           Left->Degree != Right->Degree
 * Returns: false if *Sum cannot be properly initialized, true otherwise
 */
bool Polynomial_Add(Polynomial* const Sum, const Polynomial* const Left, const Polynomial* const Right)
{
    int64_t maxDegree;
    if (Left->Degree > Right->Degree)
	{
	    maxDegree = Left->Degree;
	}
	else
	{
	    maxDegree = Right->Degree;
	}
	Sum->Degree = maxDegree;
	for (int i = 0; i <= Left->Degree; i++)
	{
	    Sum->Coeff[i] += Left->Coeff[i];
	}
	for (int j = 0; j <= Right->Degree; j++)
	{
	    Sum->Coeff[j] += Right->Coeff[j];
	}
	return true;
}

/**
 * Initializes *Diff to equal *Left - *Right.
 *
 * Pre:  Diff points to a Polynomial object,
 *       Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object,
 *       Diff != Left,
 *       Diff != Right
 * Post: Diff->Degree is set correctly,
 *       Diff->Coeff[i] == Left->Coeff[i] - Right->Coeff[i] 
 *           with proper allowance for the case that 
 *           Left->Degree != Right->Degree
 * Returns: false if *Diff cannot be properly initialized, true otherwise
 */
bool Polynomial_Subtract(Polynomial* const Diff, const Polynomial* const Left, const Polynomial* const Right)
{
    int64_t maxDegree;
    if (Left->Degree > Right->Degree)
	{
	    maxDegree = Left->Degree;
	}
	else
	{
	    maxDegree = Right->Degree;
	}
	Diff->Degree = maxDegree;
	for (int i = 0; i <= Left->Degree; i++)
	{
	    Diff->Coeff[i] += Left->Coeff[i];
	}
	for (int j = 0; j <= Right->Degree; j++)
	{
	    Diff->Coeff[j] -= Right->Coeff[j];
	}
	return true;
}

/**
 * Computes the first derivative of Source.
 *
 * Pre:  Target points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object,
 *       Target != Source
 * Post: Target->Degree is set correctly
 *       Target->Coeff[i] == iith coefficient of Source'
 *
 * Returns: false if Source' cannot be properly initialized, true otherwise
 */
bool    Polynomial_Differentiate(Polynomial* const Target, const Polynomial* const Source)
{
    Target->Degree = Source->Degree;
	for (int i = 0; i < Source->Degree; i++)
	{
	    Target->Coeff[i] = Source->Coeff[i];
	}
	return true;
}

/**
 * Reset P to represent zero polynomial.
 *
 * Pre:  P points to a Polynomial object
 * Post: P->Degree == 0
 *       P->Coeff is set appropriately
 */
bool Polynomial_Zero(Polynomial* const P)
{
    int64_t* poly = malloc(sizeof(int64_t));
	poly[0] = P->Coeff[0];
	return Polynomial_Set(P, 0, poly);
}

// On my honor:
//
// - I have not discussed the C++ language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
// - I have not used C++ language code obtained from another student,
// or any other unauthorized source, either modified or unmodified.
// - If any C++ language code or documentation used in my program
// was obtained from another source, such as a text book or course
// notes, that has been clearly noted with a proper citation in
// the comments of my program.
// - I have not designed this program in such a way as to defeat or
// interfere with the normal operation of the Curator System.
//
// <Sheng Zhou> 