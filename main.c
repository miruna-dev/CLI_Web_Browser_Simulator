/*Enache Miruna - 311 CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct page
{
	int id;
	char url[50];
	char *description;
} page;

typedef struct tab
{
	int id;
	struct page *currentPage;
	struct stack *backwardStack;
	struct stack *forwardStack;
} tab;

typedef struct stack
{
	int pagina;
	char url[50];
	struct stack *next;
} stack;

typedef struct tabsList
{
	struct tab info;
	struct tabsList *next;
	struct tabsList *prev;
} tabsList;

typedef struct browser
{
	struct tabsList *current, list;
} browser;

void pop();
void elibereaza_pagina();
void elibereaza_stiva();
void elibereaza_tab();

void seteaza_pagina(tab *info, page pagina)
{
	if (info->currentPage == NULL)
	{
		info->currentPage = calloc(1, sizeof(page));
		int len = strlen(pagina.description) + 1;
		info->currentPage->description = calloc(len, sizeof(char));
		// alocam memorie pentru pagina curenta daca nu exista
	}
	else
	{
		free(info->currentPage->description);
		// eliberam memoria
		int len=strlen(pagina.description) + 1;
		info->currentPage->description = calloc(len, sizeof(char));
		// alocam memorie
	}
	info->currentPage->id = pagina.id;
	strcpy(info->currentPage->url, pagina.url);
	strcpy(info->currentPage->description, pagina.description);
	//copiem id-ul, url-ul si descrierea
}

void new_tab(tabsList **santinela, page implicit, int *id_ultim_tab, browser **b, int ok)
{
	if (*santinela == NULL)
	{
		*santinela = calloc(1, sizeof(tabsList));
		// se creeaza primul tab
		seteaza_pagina(&(*santinela)->info, implicit);
		(*santinela)->info.backwardStack = NULL;
		(*santinela)->info.forwardStack = NULL;
		(*id_ultim_tab)++;
		(*santinela)->info.id = *id_ultim_tab;
		(*santinela)->next = *santinela;
		(*santinela)->prev = *santinela;
		// se actualizeaza tabul curent
	}
	else
	{
		tabsList *p = calloc(1, sizeof(tabsList));
		seteaza_pagina(&(p->info), implicit);
		(*id_ultim_tab)++;
		p->info.id = *id_ultim_tab;
		// se pune id-ul noului tab
		(*santinela)->prev->next = p;
		p->prev = (*santinela)->prev;
		(*santinela)->prev = p;
		p->next = *santinela;
		*santinela = p;
		// se actualizeaza tabul curent
	}
	if (ok == 1 || (*b)->current->info.id == 0)
		(*b)->current = *santinela;
	//daca nu s-a deschis nicio pagina
	//pagina curenta din browser devine tabul nou deschis
}

void print(tabsList *santinela, FILE *g)
{
	tabsList *p = santinela;
	if (p->prev != santinela)
	{
		do
		{
			fprintf(g, "%d ", p->info.id);
			p = p->prev;
		} while (p->prev != santinela);
	}
	fprintf(g, "%d", p->info.id);
}

void close(tabsList **santinela, browser **b, FILE *g)
{
	if ((*b)->current == *santinela)
	{
		if ((*santinela)->next != *santinela && (*santinela)->info.id != 0)
		{
			tabsList *p = *santinela;
			*santinela = (*santinela)->next;
			(*b)->current = *santinela;
			//se muta atat santinela cat si tabul curent
			p->next->prev = p->prev;
			p->prev->next = p->next;
			elibereaza_tab(&(p->info));
			free(p);
			//se sterge tabul din lista 
		}
		else
			fprintf(g, "%s", "403 Forbidden\n");
			//santinela este primul tab deschis care nu se poate inchide
	}
	else
	{
		if ((*b)->current->info.id != 0)
		{
			tabsList *p = (*b)->current;
			(*b)->current = (*b)->current->next;
			//se muta doar tabul curent
			p->next->prev = p->prev;
			p->prev->next = p->next;
			elibereaza_tab(&(p->info));
			free(p);
			//se sterge tabul din lista 
		}
		else
			fprintf(g, "%s", "403 Forbidden\n");
			//santinela este primul tab deschis care nu se poate inchide
	}
}

int open(tabsList **santinela, int id, browser **b, FILE *g)
{
	int ok = 0;
	if ((*santinela)->info.id != 0)
	{
		tabsList *p = *santinela;
		do
		{
			if (p->info.id == id)
			{
				ok = 1;
				(*b)->current = p;
			}
			p = p->next;
		} while (p != *santinela && ok == 0);
		//se cauta tabul cu id-ul specificat
	}

	if (ok == 0)
	{
		fprintf(g, "%s", "403 Forbidden\n");
		return 0;
	}
	return 1;
}

void prev(tabsList **tab)
{
	*tab = (*tab)->next;
}

void next(tabsList **tab)
{
	*tab = (*tab)->prev;
}

void pop(stack **s)
{
	if (*s != NULL)
	{
		stack *p = *s;
		*s = (*s)->next;
		free(p);
	}
	// se sterge elementul din stiva
}

void push(stack **s, int pagina, char url[])
{
	if (*s == NULL)
	{
		*s = calloc(1, sizeof(stack));
		(*s)->pagina = pagina;
		strcpy((*s)->url, url);
		(*s)->next = NULL;
	}
	else
	{
		stack *p = calloc(1, sizeof(stack));
		p->pagina = pagina;
		strcpy(p->url, url);
		p->next = *s;
		*s = p;
	}
	// se adauga elementul in stiva
}

void afisare_stiva_invers(stack *s, FILE *g)
{
	if (s == NULL)
		return;
	afisare_stiva_invers(s->next, g);
	fprintf(g, "%s\n", s->url);
	// afisare stiva invers
}

void afisare_stiva(stack *s, FILE *g)
{
	while (s != NULL)
	{
		fprintf(g, "%s\n", s->url);
		s = s->next;
	}
	// afisare stiva
}

void page_id(tabsList **tab, page pagina[], int id, int nr_pagini, FILE *g)
{
	int i, ok = 0;
	for (i = 0; i < nr_pagini && ok == 0; i++)
	{
		if (pagina[i].id == id)
		{
			ok = 1;
			if ((*tab)->info.currentPage != NULL)
			{
				push(&((*tab)->info.backwardStack),
					 (*tab)->info.currentPage->id,
					 (*tab)->info.currentPage->url);
				// se adauga pagina curenta in backwardStack
			}

			elibereaza_stiva(&((*tab)->info.forwardStack));
			// se goleste stiva forwardStack
			seteaza_pagina(&((*tab)->info), pagina[i]);
			// se seteaza pagina curenta
		}
	}
	if (ok == 0)
		fprintf(g, "403 Forbidden\n");
	// pagina nu exista
}

void print_history(tabsList *santinela, int id, FILE *g)
{
	if (santinela != NULL)
	{
		tabsList *p = santinela;
		int ok = 0;
		p = p->next;
		while (p != santinela && ok == 0)
		{
			if (p->info.id == id)
				ok = 1;
			p = p->next;
		}
		//se cauta tabul cu id-ul cerut in lista 
		if (ok == 1 || id == 0)
		{
			//daca tabul a fost gasit sau daca este chiar primul tab 
			afisare_stiva_invers(p->info.forwardStack, g);
			fprintf(g, "%s\n", p->info.currentPage->url);
			afisare_stiva(p->info.backwardStack, g);
		}
		else
			fprintf(g, "403 Forbidden\n");
	}
	else
		fprintf(g, "403 Forbidden\n");
}

void backward(tabsList **tab, page pagina[], int nr_pagini, FILE *g)
{
	if ((*tab)->info.backwardStack != NULL)
	{
		//verificam daca avem pagini in backward
		push(&((*tab)->info.forwardStack),
			 (*tab)->info.currentPage->id,
			 (*tab)->info.currentPage->url);
		// se adauga pagina curenta in forwardStack
		int id = (*tab)->info.backwardStack->pagina;
		pop(&((*tab)->info.backwardStack)); 
		// se sterge pagina noua din backwardStack
		int i, ok = 0;
		for (i = 0; i < nr_pagini && ok == 0; i++)
		{
			if (pagina[i].id == id)
				seteaza_pagina(&(*tab)->info, pagina[i]), ok = 1;
			//se seteaza drept curenta pagina extrasa din backward
		}
	}
	else
	{
		fprintf(g, "403 Forbidden\n");
		(*tab)->info.currentPage->id=0;
		strcpy((*tab)->info.currentPage->url, "https://acs.pub.ro/");
		strcpy((*tab)->info.currentPage->description, "Computer Science\n");
		//se stereaza pagina la pagina standard
	}
	// nu exista pagina inapoi
}

void forward(tabsList **santinela, page pagina[], int nr_pagini, FILE *g)
{
	if ((*santinela)->info.forwardStack != NULL)
	{
		push(&((*santinela)->info.backwardStack), (*santinela)->info.currentPage->id, (*santinela)->info.currentPage->url);
		// se adauga pagina curenta in backwardStack
		int id = (*santinela)->info.forwardStack->pagina;
		pop(&((*santinela)->info.forwardStack));
		// se sterge pagina noua din forwardStack
		int i, ok = 0;
		for (i = 0; i < nr_pagini && ok == 0; i++)
		{
			if (pagina[i].id == id)
				seteaza_pagina(&(*santinela)->info, pagina[i]), ok = 1;
		}
	}
	else
		fprintf(g, "403 Forbidden\n");
	// nu exista pagina inainte
}

void elibereaza_pagina(page *pagina)
{
	if (pagina != NULL)
	{
		free(pagina->description);
	}
	//se goleste pagina
}

void elibereaza_stiva(stack **s)
{
	while (*s != NULL)
		pop(s);
	// se goleste stiva
}

void elibereaza_tab(tab *t)
{
	if (t != NULL)
	{
		elibereaza_pagina(t->currentPage);
		elibereaza_stiva(&t->backwardStack);
		elibereaza_stiva(&t->forwardStack);
		free(t->currentPage);
	}
}

void elibereaza_tabList(tabsList **santinela)
{
	if (*santinela == NULL)
		return;

	tabsList *p = (*santinela)->next;
	while (p != *santinela)
	{
		tabsList *q = p;
		p = p->next;
		elibereaza_tab(&q->info);
		free(q);
	}
	elibereaza_tab((&(*santinela)->info));
	free(*santinela);
}

int main()
{
	FILE *f = fopen("tema1.in", "r");
	FILE *g = fopen("tema1.out", "w");
	page pagina[50];
	tabsList *santinela = NULL;
	int nr_pagini, i, nr_operatii, id_ultim_tab = -1, ok = 1;
	browser *b = NULL;
	b = calloc(1, sizeof(browser));
	fscanf(f, "%d", &nr_pagini);
	fgetc(f);

	for (i = 0; i < nr_pagini; i++)
	{
		fscanf(f, "%d", &pagina[i].id);
		fgetc(f);
		fgets(pagina[i].url, 50, f);
		pagina[i].url[strlen(pagina[i].url) - 1] = '\0';
		// elimina newline
		pagina[i].description = calloc(200, sizeof(char));
		fgets(pagina[i].description, 200, f);
		pagina[i].description[strlen(pagina[i].description) - 1] = '\0';
		strcat(pagina[i].description, "\n");
		int lungime = strlen(pagina[i].description);
		pagina[i].description = realloc(pagina[i].description, lungime + 1);
		// realloc pentru a elimina spatiile alocate in plus
	}

	page implicit;
	implicit.id = 0;
	strcpy(implicit.url, "https://acs.pub.ro/");
	implicit.description = calloc(20, sizeof(char));
	strcpy(implicit.description, "Computer Science\n");

	fscanf(f, "%d", &nr_operatii);
	fgetc(f);

	new_tab(&santinela, implicit, &id_ultim_tab, &b, ok);
	for (i = 0; i < nr_operatii; i++)
	{
		char operatie[50], *auxid;
		int id;
		fgets(operatie, 50, f);
		//   citeste operatia
		//  elimina newline
		if (strstr(operatie, "NEW_TAB"))
			new_tab(&santinela, implicit, &id_ultim_tab, &b, ok); 
		else if (strstr(operatie, "CLOSE"))
			close(&santinela, &b, g); 
		else if (strstr(operatie, "OPEN"))
		{
			int k;
			auxid = strtok(operatie, " ");
			auxid = strtok(NULL, " ");
			id = atoi(auxid);
			// extragem id-ul
			k = open(&santinela, id, &b, g); 
			if (ok == 1)// nu s-a efectuat niciun open pana acum
			{
				if (k == 0) // open nu a reusit
					ok = 1; // browserul se afla tot pe pozitia santinelei
				else
					ok = 0;
			}
			else
				ok = 0; // browserul si-a modificat pozitia
		}
		else if (strstr(operatie, "PREV"))
			prev(&(b->current)); // printf("se executa prev\n");
		else if (strstr(operatie, "NEXT"))
			next(&(b->current)); // printf("se executa next\n");
		else if (strstr(operatie, "PAGE"))
		{
			auxid = strtok(operatie, " ");
			auxid = strtok(NULL, " ");
			id = atoi(auxid);
			page_id(&(b->current), pagina, id, nr_pagini, g);
		}
		else if (strstr(operatie, "PRINT_HISTORY"))
		{
			auxid = strtok(operatie, " ");
			auxid = strtok(NULL, " ");
			id = atoi(auxid);
			print_history(b->current, id, g);
		}
		else if (strstr(operatie, "PRINT"))
		{
			print(b->current, g);
			fprintf(g, "\n%s", b->current->info.currentPage->description);
		}
		else if (strstr(operatie, "BACKWARD"))
			backward(&santinela, pagina, nr_pagini, g);
		else
			forward(&santinela, pagina, nr_pagini, g);
	}
	for (i = 0; i < nr_pagini; i++)
		elibereaza_pagina(&(pagina[i]));

	elibereaza_tabList(&santinela);
	free(implicit.description);
	free(b);
	fclose(f);
	fclose(g);
	return 0;
}