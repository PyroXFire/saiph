#ifndef HEALTH_H
/* defines */
#define HEALTH_H
/* priorities */
#define HEALTH_PRAY_FOR_FOOD 1000
#define HEALTH_REST_FOR_HP 800

/* forward declare */
class Health;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Health : public Analyzer {
	public:
		/* constructors */
		Health(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();

	private:
		/* variables */
		Saiph *saiph;
		string action;
		bool resting;
		Request request;
};
#endif
