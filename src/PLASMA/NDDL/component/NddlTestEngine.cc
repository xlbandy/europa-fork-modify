
#include "NddlTestEngine.hh"

#include "Error.hh"
#include "ConstraintEngine.hh"
#include "PlanDatabase.hh"
#include "PlanDatabaseWriter.hh"

// Support for registered constraints
#include "ConstraintFactory.hh"
#include "Constraints.hh"
#include "Propagators.hh"
#include "CommonAncestorConstraint.hh"
#include "HasAncestorConstraint.hh"

// Misc
#include "ModuleConstraintEngine.hh"
#include "ModulePlanDatabase.hh"
#include "ModuleTemporalNetwork.hh"
#include "ModuleRulesEngine.hh"
#include "ModuleNddl.hh"

void initialize(CESchema* ces)
{
  /*
   *  TODO: constraint registration below needs to be removed, ModuleConstraintLibrary::initialize takes care of this
   *  leaving it for now for backwards compatibility since some constraints are named differently
   *  and some other constraints like Lock and Ancestor are not registered by ModuleConstraintLibrary::initialize for some reason
   */

  // Procedural Constraints used with Default Propagation
  REGISTER_CONSTRAINT(ces,EqualConstraint, "eq", "Default");
  REGISTER_CONSTRAINT(ces,NotEqualConstraint, "neq", "Default");
  REGISTER_CONSTRAINT(ces,LessThanEqualConstraint, "leq", "Default");
  REGISTER_CONSTRAINT(ces,LessThanConstraint, "lessThan", "Default");
  REGISTER_CONSTRAINT(ces,AddEqualConstraint, "addEq", "Default");
  REGISTER_CONSTRAINT(ces,NegateConstraint, "neg", "Default");
  REGISTER_CONSTRAINT(ces,MultEqualConstraint, "mulEq", "Default");
  REGISTER_CONSTRAINT(ces,AddMultEqualConstraint, "addMulEq", "Default");
  REGISTER_CONSTRAINT(ces,AddMultEqualConstraint, "addmuleq", "Default");
  REGISTER_CONSTRAINT(ces,SubsetOfConstraint, "subsetOf", "Default");
  REGISTER_CONSTRAINT(ces,SubsetOfConstraint, "Singleton", "Default");
  REGISTER_CONSTRAINT(ces,LockConstraint, "Lock", "Default");
  REGISTER_CONSTRAINT(ces,CommonAncestorConstraint, "commonAncestor", "Default");
  REGISTER_CONSTRAINT(ces,HasAncestorConstraint, "hasAncestor", "Default");
  REGISTER_CONSTRAINT(ces,TestEQ, "testEQ", "Default");
  REGISTER_CONSTRAINT(ces,TestLEQ, "testLEQ", "Default");
  REGISTER_CONSTRAINT(ces,EqualSumConstraint, "sum", "Default");
}

NddlTestEngine::NddlTestEngine()
{
    Error::doThrowExceptions(); // throw exceptions!
}

void NddlTestEngine::init()
{
    createModules();
    doStart();
    initialize((CESchema*)getComponent("CESchema"));
}

NddlTestEngine::~NddlTestEngine()
{
    doShutdown();
}

void NddlTestEngine::createModules()
{
    addModule((new ModuleConstraintEngine())->getId());
    addModule((new ModuleConstraintLibrary())->getId());
    addModule((new ModulePlanDatabase())->getId());
    addModule((new ModuleRulesEngine())->getId());
    addModule((new ModuleTemporalNetwork())->getId());
    addModule((new ModuleNddl())->getId());
}

int NddlTestEngine::run(int argc, const char** argv)
{
  if (argc != 3) {
    std::cerr << "Must provide model file and language to interpret" << std::endl;
    return -1;
  }

  const char* txSource = argv[1];
  const char* language = argv[2];

  init();
  run(txSource,language);

  return 0;
}

void NddlTestEngine::run(const char* txSource, const char* language)
{
    PlanDatabase* planDatabase = (PlanDatabase*) getComponent("PlanDatabase");
    planDatabase->getClient()->enableTransactionLogging();

    executeScript(language,txSource,true /*isFile*/);

    ConstraintEngine* ce = (ConstraintEngine*)getComponent("ConstraintEngine");
    assert(ce->constraintConsistent());

    PlanDatabaseWriter::write(planDatabase->getId(), std::cout);
    std::cout << "Finished\n";
}