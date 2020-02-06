/* Compiler and system dependent definitions: */

#define NUMBER_OF_RUNS 100 /* Default number of runs */

#define Ident_1 0
#define Ident_2 1
#define Ident_3 2
#define Ident_4 3
#define Ident_5 4

/* General definitions: */

/* for strcpy, strcmp */
#include <stdio.h>
#include <string.h>

/* Value of a Null pointer */
#define Null 0 

#define true  1
#define false 0

typedef char    Str_30 [31];
typedef int     Arr_1_Dim [50];
typedef int     Arr_2_Dim [50] [50];

typedef struct record 
{
	struct record *Ptr_Comp;
	int Discr;
	union {
		struct {
			int Enum_Comp;
			int Int_Comp;
			char Str_Comp [31];
		} var_1;
		struct {
			int E_Comp_2;
			char Str_2_Comp [31];
		} var_2;
		struct {
			char Ch_1_Comp;
			char Ch_2_Comp;
		} var_3;
	} variant;
} Rec_Type, *Rec_Pointer;

/* Global Variables: */

Rec_Type Rec, Rec_Next;
Rec_Pointer Ptr_Glob, Next_Ptr_Glob;
int Int_Glob;
int Bool_Glob;
char Ch_1_Glob, Ch_2_Glob;
int Arr_1_Glob [50];
int Arr_2_Glob [50][50];

/* Function prototypes */
void memcpy_custom (char *d, char *s, int l);
void Proc_1 (Rec_Pointer Ptr_Val_Par);
void Proc_2 (int *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
void Proc_4 (void);
void Proc_5 (void);
void Proc_6 (int Enum_Val_Par, int *Enum_Ref_Par);
void Proc_7 (int Int_1_Par_Val, int Int_2_Par_Val, int *Int_Par_Ref);
void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref,
	     int Int_1_Par_Val, int Int_2_Par_Val);
int Func_1 (char Ch_1_Par_Val, char Ch_2_Par_Val);
int Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref);
int Func_3 (int Enum_Par_Val);

void main () 
{
        int Int_1_Loc;
	int Int_2_Loc;
        int Int_3_Loc;
	char Ch_Index;
        int Enum_Loc;
        Str_30 Str_1_Loc;
        Str_30 Str_2_Loc;
	int Run_Index;
	int Number_Of_Runs = NUMBER_OF_RUNS;

	/* Initializations */

	Ptr_Glob = &Rec;
	Next_Ptr_Glob = &Rec_Next;

	Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
	Ptr_Glob->Discr                       = Ident_1;
	Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
	Ptr_Glob->variant.var_1.Int_Comp      = 40;
	strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
		"DHRYSTONE PROGRAM, SOME STRING");
	strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

	Arr_2_Glob[8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

	int Done = false;
	while (!Done) {

		/* printf ("Trying %d runs through Dhrystone:\n", Number_Of_Runs); */

		for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
		{
			Proc_5();
			Proc_4();
			/* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
			Int_1_Loc = 2;
			Int_2_Loc = 3;
			strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
			Enum_Loc = Ident_2;
			Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
			/* Bool_Glob == 1 */
			while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
			{
				Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
				/* Int_3_Loc == 7 */
				Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
				/* Int_3_Loc == 7 */
				Int_1_Loc += 1;
			} /* while */
			/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
			Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
			/* Int_Glob == 5 */
			Proc_1 (Ptr_Glob);
			for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
				/* loop body executed twice */
			{
				if (Enum_Loc == Func_1 (Ch_Index, 'C'))
					/* then, not executed */
				{
					Proc_6 (Ident_1, &Enum_Loc);
					strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
					Int_2_Loc = Run_Index;
					Int_Glob = Run_Index;
				}
			}
			/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
			Int_2_Loc = Int_2_Loc * Int_1_Loc;
			Int_1_Loc = Int_2_Loc / Int_3_Loc;
			Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
			/* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
			Proc_2 (&Int_1_Loc);
			/* Int_1_Loc == 5 */

		} /* loop "for Run_Index" */

		Done = true;
	}

	/* fprintf (stderr, "Final values of the variables used in the benchmark:\n"); */
	/* fprintf (stderr, "\n"); */
	/* fprintf (stderr, "Int_Glob:            %d\n", Int_Glob); */
	/* fprintf (stderr, "        should be:   %d\n", 5); */
	/* fprintf (stderr, "Bool_Glob:           %d\n", Bool_Glob); */
	/* fprintf (stderr, "        should be:   %d\n", 1); */
	/* fprintf (stderr, "Ch_1_Glob:           %c\n", Ch_1_Glob); */
	/* fprintf (stderr, "        should be:   %c\n", 'A'); */
	/* fprintf (stderr, "Ch_2_Glob:           %c\n", Ch_2_Glob); */
	/* fprintf (stderr, "        should be:   %c\n", 'B'); */
	/* fprintf (stderr, "Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]); */
	/* fprintf (stderr, "        should be:   %d\n", 7); */
	/* fprintf (stderr, "Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]); */
	/* fprintf (stderr, "        should be:   Number_Of_Runs + 10\n"); */
	/* fprintf (stderr, "Ptr_Glob->\n"); */
	/* /\* fprintf (stderr, "  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp); *\/ */
	/* /\* fprintf (stderr, "        should be:   (implementation-dependent)\n"); *\/ */
	/* fprintf (stderr, "  Discr:             %d\n", Ptr_Glob->Discr); */
	/* fprintf (stderr, "        should be:   %d\n", 0); */
	/* fprintf (stderr, "  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp); */
	/* fprintf (stderr, "        should be:   %d\n", 2); */
	/* fprintf (stderr, "  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp); */
	/* fprintf (stderr, "        should be:   %d\n", 17); */
	/* fprintf (stderr, "  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp); */
	/* fprintf (stderr, "        should be:   DHRYSTONE PROGRAM, SOME STRING\n"); */
	/* fprintf (stderr, "Next_Ptr_Glob->\n"); */
	/* /\* fprintf (stderr, "  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp); *\/ */
	/* /\* fprintf (stderr, "        should be:   (implementation-dependent), same as above\n"); *\/ */
	/* fprintf (stderr, "  Discr:             %d\n", Next_Ptr_Glob->Discr); */
	/* fprintf (stderr, "        should be:   %d\n", 0); */
	/* fprintf (stderr, "  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp); */
	/* fprintf (stderr, "        should be:   %d\n", 1); */
	/* fprintf (stderr, "  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp); */
	/* fprintf (stderr, "        should be:   %d\n", 18); */
	/* fprintf (stderr, "  Str_Comp:          %s\n", */
	/* 	 Next_Ptr_Glob->variant.var_1.Str_Comp); */
	/* fprintf (stderr, "        should be:   DHRYSTONE PROGRAM, SOME STRING\n"); */
	/* fprintf (stderr, "Int_1_Loc:           %d\n", Int_1_Loc); */
	/* fprintf (stderr, "        should be:   %d\n", 5); */
	/* fprintf (stderr, "Int_2_Loc:           %d\n", Int_2_Loc); */
	/* fprintf (stderr, "        should be:   %d\n", 13); */
	/* fprintf (stderr, "Int_3_Loc:           %d\n", Int_3_Loc); */
	/* fprintf (stderr, "        should be:   %d\n", 7); */
	/* fprintf (stderr, "Enum_Loc:            %d\n", Enum_Loc); */
	/* fprintf (stderr, "        should be:   %d\n", 1); */
	/* fprintf (stderr, "Str_1_Loc:           %s\n", Str_1_Loc); */
	/* fprintf (stderr, "        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n"); */
	/* fprintf (stderr, "Str_2_Loc:           %s\n", Str_2_Loc); */
	/* fprintf (stderr, "        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n"); */
	/* fprintf (stderr, "\n"); */
}

void Proc_1 (Rec_Pointer Ptr_Val_Par)
{
	Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
	/* == Ptr_Glob_Next */
	/* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
	/* corresponds to "rename" in Ada, "with" in Pascal           */
  
	memcpy_custom( (char *) Ptr_Val_Par->Ptr_Comp, (char *) Ptr_Glob, sizeof(struct record)); 
	Ptr_Val_Par->variant.var_1.Int_Comp = 5;
	Next_Record->variant.var_1.Int_Comp 
		= Ptr_Val_Par->variant.var_1.Int_Comp;
	Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
	Proc_3 (&Next_Record->Ptr_Comp);
	/* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
	   == Ptr_Glob->Ptr_Comp */
	if (Next_Record->Discr == Ident_1)
		/* then, executed */
	{
		Next_Record->variant.var_1.Int_Comp = 6;
		Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
			&Next_Record->variant.var_1.Enum_Comp);
		Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
		Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
			&Next_Record->variant.var_1.Int_Comp);
	}
	else /* not executed */
		memcpy_custom((char *) Ptr_Val_Par, (char *) Ptr_Val_Par->Ptr_Comp, sizeof(struct record));
} /* Proc_1 */


void Proc_2(int   *Int_Par_Ref)
{
	int  Int_Loc;  
	int   Enum_Loc;

	Int_Loc = *Int_Par_Ref + 10;
	do /* executed once */
		if (Ch_1_Glob == 'A')
			/* then, executed */
		{
			Int_Loc -= 1;
			*Int_Par_Ref = Int_Loc - Int_Glob;
			Enum_Loc = Ident_1;
		} /* if */
	while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Rec_Pointer *Ptr_Ref_Par)
/******************/
/* executed once */
/* Ptr_Ref_Par becomes Ptr_Glob */
{
	if (Ptr_Glob != Null)
		/* then, executed */
		*Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
	Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 (void)
/*******/
/* executed once */
{
	int Bool_Loc;

	Bool_Loc = Ch_1_Glob == 'A';
	Bool_Glob = Bool_Loc | Bool_Glob;
	Ch_2_Glob = 'B';
} /* Proc_4 */

void Proc_5 (void) /* without parameters */
/*******/
/* executed once */
{
	Ch_1_Glob = 'A';
	Bool_Glob = false;
} /* Proc_5 */


/* Procedure for the assignment of structures,          */
/* if the C compiler doesn't support this feature       */
void memcpy_custom (char *d, char *s, int l)
{
        while (l--) *d++ = *s++;
}

void Proc_6 (int Enum_Val_Par, int *Enum_Ref_Par)
{
	*Enum_Ref_Par = Enum_Val_Par;
	if (! Func_3 (Enum_Val_Par))
		/* then, not executed */
		*Enum_Ref_Par = Ident_4;
	switch (Enum_Val_Par)
	{
	case Ident_1: 
		*Enum_Ref_Par = Ident_1;
		break;
	case Ident_2: 
		if (Int_Glob > 100)
			/* then */
			*Enum_Ref_Par = Ident_1;
		else *Enum_Ref_Par = Ident_4;
		break;
	case Ident_3: /* executed */
		*Enum_Ref_Par = Ident_2;
		break;
	case Ident_4: break;
	case Ident_5: 
		*Enum_Ref_Par = Ident_3;
		break;
	} /* switch */
} /* Proc_6 */


void Proc_7 (int Int_1_Par_Val, int Int_2_Par_Val, int *Int_Par_Ref)
/**********************************************/
/* executed three times                                      */ 
/* first call:      Int_1_Par_Val == 2, Int_2_Par_Val == 3,  */
/*                  Int_Par_Ref becomes 7                    */
/* second call:     Int_1_Par_Val == 10, Int_2_Par_Val == 5, */
/*                  Int_Par_Ref becomes 17                   */
/* third call:      Int_1_Par_Val == 6, Int_2_Par_Val == 10, */
/*                  Int_Par_Ref becomes 18                   */
{
	int Int_Loc;

	Int_Loc = Int_1_Par_Val + 2;
	*Int_Par_Ref = Int_2_Par_Val + Int_Loc;
} /* Proc_7 */


void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref,
	     int Int_1_Par_Val, int Int_2_Par_Val)
/*********************************************************************/
/* executed once      */
/* Int_Par_Val_1 == 3 */
/* Int_Par_Val_2 == 7 */
{
	int Int_Index;
	int Int_Loc;

	Int_Loc = Int_1_Par_Val + 5;
	Arr_1_Par_Ref [Int_Loc] = Int_2_Par_Val;
	Arr_1_Par_Ref [Int_Loc+1] = Arr_1_Par_Ref [Int_Loc];
	Arr_1_Par_Ref [Int_Loc+30] = Int_Loc;
	for (Int_Index = Int_Loc; Int_Index <= Int_Loc+1; ++Int_Index)
		Arr_2_Par_Ref [Int_Loc] [Int_Index] = Int_Loc;
	Arr_2_Par_Ref [Int_Loc] [Int_Loc-1] += 1;
	Arr_2_Par_Ref [Int_Loc+20] [Int_Loc] = Arr_1_Par_Ref [Int_Loc];
	Int_Glob = 5;
} /* Proc_8 */


int Func_1 (char Ch_1_Par_Val, char Ch_2_Par_Val)
/*************************************************/
/* executed three times                                         */
/* first call:      Ch_1_Par_Val == 'H', Ch_2_Par_Val == 'R'    */
/* second call:     Ch_1_Par_Val == 'A', Ch_2_Par_Val == 'C'    */
/* third call:      Ch_1_Par_Val == 'B', Ch_2_Par_Val == 'C'    */
{
	char        Ch_1_Loc;
	char        Ch_2_Loc;

	Ch_1_Loc = Ch_1_Par_Val;
	Ch_2_Loc = Ch_1_Loc;
	if (Ch_2_Loc != Ch_2_Par_Val)
		/* then, executed */
		return (Ident_1);
	else  /* not executed */
	{
		Ch_1_Glob = Ch_1_Loc;
		return (Ident_2);
	}
} /* Func_1 */


int Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref)
/*************************************************/
/* executed once */
/* Str_1_Par_Ref == "DHRYSTONE PROGRAM, 1'ST STRING" */
/* Str_2_Par_Ref == "DHRYSTONE PROGRAM, 2'ND STRING" */
{
	int        Int_Loc;
	char    Ch_Loc;

	Int_Loc = 2;
	while (Int_Loc <= 2) /* loop body executed once */
		if (Func_1 (Str_1_Par_Ref[Int_Loc],
			    Str_2_Par_Ref[Int_Loc+1]) == Ident_1)
			/* then, executed */
		{
			Ch_Loc = 'A';
			Int_Loc += 1;
		} /* if, while */
	if (Ch_Loc >= 'W' && Ch_Loc < 'Z')
		/* then, not executed */
		Int_Loc = 7;
	if (Ch_Loc == 'R')
		/* then, not executed */
		return (true);
	else /* executed */
	{
		if (strcmp (Str_1_Par_Ref, Str_2_Par_Ref) > 0)
			/* then, not executed */
		{
			Int_Loc += 7;
			Int_Glob = Int_Loc;
			return (true);
		}
		else /* executed */
			return (false);
	} /* if Ch_Loc */
} /* Func_2 */


int Func_3 (int Enum_Par_Val)
/***************************/
/* executed once        */
/* Enum_Par_Val == Ident_3 */
{
	int Enum_Loc;

	Enum_Loc = Enum_Par_Val;
	if (Enum_Loc == Ident_3)
		/* then, executed */
		return (true);
	else /* not executed */
		return (false);
} /* Func_3 */
