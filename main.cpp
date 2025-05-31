#include "UI.h"
#include "Load.h"
#include "Output.h"

#define FINAL_SIMULATION
#define MAX_NEW_APP_ACCEPTANCE_TIME 100


#ifdef FINAL_SIMULATION

int main()
{
	//call the srand function and give it the seed once
	srand(RANDOM_ARGUMENT);

	Schedular hospitalSystem;

	Load loader("Miscellinious_Test_2");

	// Load data from file into hospitalSystem
	loader.LoadDataFromFile(hospitalSystem);

	UI::DisplayAllPatients(hospitalSystem);
	UI::DisplayResources(hospitalSystem);
	// ====================== Simulation Run ======================
	cout << "===== Treatment Sequence Test =====" << endl;
	while (!hospitalSystem.SimulationEnded())
	{
		if (hospitalSystem.getCurrentTime() < MAX_NEW_APP_ACCEPTANCE_TIME)
		{
			loader.GenerateRandomPatient(hospitalSystem);
			cout << endl << endl;
		}
		// Core processing sequence
		hospitalSystem.CheckMaintainanceList();
		hospitalSystem.MoveToNextTreatment();

		hospitalSystem.CheckAllPatientList();
		hospitalSystem.CheckInterrupted();
		hospitalSystem.EarlyToWait();
		hospitalSystem.LateToWait();
		hospitalSystem.WaitToTreatment();

		UI::DisplaySystemStatus(hospitalSystem);

		hospitalSystem.IncrementTime();

		//if (hospitalSystem.getCurrentTime() > 300)
		//	break;
	}

	UI::DisplayResources(hospitalSystem);
	UI::displayHistory(hospitalSystem);

	// ====================== Final Status Report ======================
	UI::displayFinalPatient(hospitalSystem);

	// ====================== Generate Output File ======================
	Output outputHandler("Miscellinious_Test_2_output.txt");
	outputHandler.GenerateOutputFile("Miscellinious_Test_2_output.txt", hospitalSystem);

	cout << "\nOutput file 'Miscellinious_Test_2_output.txt' has been generated successfully.\n";

	return 0;
}

#endif // end FINAL_SIMULATION
