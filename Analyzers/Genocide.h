#ifndef GENOCIDE_H
#define	GENOCIDE_H

#include <string>
#include "../Analyzer.h"
#include "../Saiph.h"

class Genocide : public Analyzer {
	public:
		Genocide(Saiph* saiph);
		void parseMessages(const std::string& messages);

	private:
		Saiph* saiph;
		bool genoedL, genoedSemicolon;
		bool genoedRegularFlayer, genoedMasterFlayer, genoedRust, genoedDisenchant;
};
#endif