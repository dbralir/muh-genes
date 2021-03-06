#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
using namespace std;

class Gene {
  private:
    int gene_size;
  public:
    vector <bool> bases;
    Gene();
    void gene_setup(int igene_size);
    void print();
    void randomize();
    bool set_base(int i, bool val);
    int get_size();
};

class Genome {
  private:
    int genome_size;
    int gene_size;
  public:
    double fitness;
    vector <Gene> genes;
    Genome();
    void genome_setup(int igenome_size, int igene_size);
    void print();
};

class Population {
  private:
    int genome_size;
    int gene_size;
  public:
    int population_size;
    list <Genome> genomes;
    Population();
    void population_setup(int in, int igenome_n, int igene_n);
    void breed_ongene();
    void evaluate();
    void add_genome();
    void delete_genome(int n);
    void print();
    void print_avg_fitness();
    double get_avg_fitness();
};

void Population::evaluate()
{
  int sum;
  list <Genome>::iterator lit;
  for (lit = genomes.begin(); lit != genomes.end(); lit++) {
    if (lit->fitness == -1.0) {
      sum = 0;
      for (int j = 0; j < genome_size; j++) {
        for (int k = 0; k < gene_size; k++) {
          sum += lit->genes[j].bases[k];
        }
      }
      if (sum != 5) {
        lit->fitness = 1.0 / abs(5 - sum);
      } else {
        lit->fitness = 1.0;
      }
    }
  }
}

void Population::print_avg_fitness()
{
  evaluate();
  double sum = 0;
  list <Genome>::iterator lit;

  for (lit = genomes.begin(); lit != genomes.end(); lit++) {
    if (lit->fitness > 0 && lit->fitness <= 1) {
      sum += lit->fitness;
    }
  }
  printf("Avg fitness: %f\n", sum / population_size);
}

double Population::get_avg_fitness()
{
  evaluate();
  double sum = 0;
  list <Genome>::iterator lit;

  for (lit = genomes.begin(); lit != genomes.end(); lit++) {
    if (lit->fitness > 0 && lit->fitness <= 1) {
      sum += lit->fitness;
    }
  }
  return sum / population_size;
}

void Population::breed_ongene()
{
  double time_st, time_end;

  time_st = clock();
  evaluate();
  time_end = clock();

  time_st = clock();

  list <Genome>::iterator lit;
  vector <list <Genome>::iterator> lits;
  bool *breed_chart = new bool[population_size];
  int *breed_genomes = new int[population_size / 2];
  int selected = 0;

  for (int i = 0; i < population_size; i++) {
    breed_chart[i] = false;
  }

  int i = 0;

  while (selected < population_size / 2) {
    i = 0;
    for (lit = genomes.begin(); lit != genomes.end(); lit++) {
      if (breed_chart[i] == false) {
        if (selected >= population_size / 2) break;

        if (lit->fitness > drand48()) {
          breed_chart[i] = true;
          breed_genomes[selected] = i;
          lits.push_back(lit);
          selected++;
        }
      }
      i++;
    }
  }

  time_end = clock();


  int bred = 0;
  int breed_one;
  int breed_two;
  int fuse_gene;
  int original_size = population_size;

  time_st = clock();

  while (bred < original_size / 2) {
    breed_one = lrand48() % (original_size / 2);
    breed_two = lrand48() % (original_size / 2);

    fuse_gene = lrand48() % genome_size;

    add_genome();

      for (int i = 0; i < genome_size; i++) {
        if (i < fuse_gene) {
          genomes.back().genes[i] = lits[breed_one]->genes[i];
        } else {
          genomes.back().genes[i] = lits[breed_two]->genes[i];
      }
    }
    bred++;
  }
  time_end = clock();


  time_st = clock();

  for (int i = bred; i > 0; i--) {
    delete_genome(0);
  }

  time_end = clock();


  delete[] breed_chart;
  delete[] breed_genomes;
}

void Population::add_genome()
{
  Genome temp;
  temp.genome_setup(genome_size, gene_size);
  genomes.push_back(temp);
}

void Population::delete_genome(int n)
{
  genomes.pop_front();
}

Population::Population()
{
  srand48(time(0));
}

Gene::Gene()
{
}

void Gene::gene_setup(int igene_size)
{
  gene_size = igene_size;
  for (int i = 0; i < gene_size; i++) {
    bases.push_back(0);
  }
  randomize();
}

Genome::Genome()
{
  fitness = -1.0;
}

void Genome::genome_setup(int igenome_size, int igene_size)
{
  genome_size = igenome_size;
  gene_size = igene_size;

  Gene temp;

  for (int i = 0; i < genome_size; i++) {
    genes.push_back(temp);
    genes[i].gene_setup(gene_size);
  }
}

void Population::population_setup(int ipopulation_size, int igenome_size, int igene_size)
{
  population_size = ipopulation_size;
  genome_size = igenome_size;
  gene_size = igene_size;

  Genome temp;
  list <Genome>::iterator lit;

  for (int i = 0; i < population_size; i++) {
    genomes.push_back(temp);
    (genomes.back()).genome_setup(genome_size, gene_size);
  }
}

void Population::print()
{
  list <Genome>::iterator lit;
  for (lit = genomes.begin(); lit != genomes.end(); lit++) {
    lit->print();
  }
}

void Genome::print()
{
  for (int i = 0; i < genome_size; i++) {
    genes[i].print();
  }
  if (fitness != -1.0) {
    printf(" fitness: %f", fitness);
  }
  printf("\n");
}


void Gene::print()
{
  for (int i = 0; i < gene_size; i++) {
    printf("%d", static_cast <bool> (bases[i]));
  }
  printf(" ");
}

void Gene::randomize()
{
  for (int i = 0; i < gene_size; i++) {
    if (drand48() > 0.5) {
      bases[i] = 1;
    } else {
      bases[i] = 0;
    }
  }
}

bool Gene::set_base(int i, bool val)
{
  if (i < gene_size && i >= 0) {
    bases[i] = val;
    return true;
  } else {
    return false;
  }
}

int Gene::get_size()
{
  return gene_size;
}
