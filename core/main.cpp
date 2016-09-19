//
//  main.cpp
//  SLiM
//
//  Created by Ben Haller on 12/12/14.
//  Copyright (c) 2014-2016 Philipp Messer.  All rights reserved.
//	A product of the Messer Lab, http://messerlab.org/slim/
//

//	This file is part of SLiM.
//
//	SLiM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
//	SLiM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License along with SLiM.  If not, see <http://www.gnu.org/licenses/>.

/*
 
 This file defines main() and related functions that initiate and run a SLiM simulation.
 
 */


#include <iostream>

#include "slim_sim.h"
#include "slim_global.h"
#include "eidos_test.h"
#include "slim_test.h"


void PrintUsageAndDie(bool p_print_header, bool p_print_full_usage);

void PrintUsageAndDie(bool p_print_header, bool p_print_full_usage)
{
	if (p_print_header)
	{
		SLIM_OUTSTREAM << "SLiM version 2.1, built " << __DATE__ << " " __TIME__ << "." << std::endl << std::endl;	// SLIM VERSION
		
		SLIM_OUTSTREAM << "SLiM is a product of the Messer Lab, http://messerlab.org/" << std::endl;
		SLIM_OUTSTREAM << "Copyright 2016 Philipp Messer.  All rights reserved." << std::endl << std::endl;
		SLIM_OUTSTREAM << "By Benjamin C. Haller, http://benhaller.com/, and Philipp Messer." << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "---------------------------------------------------------------------------------" << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "SLiM home page: http://messerlab.org/slim/" << std::endl;
		SLIM_OUTSTREAM << "slim-announce mailing list: https://groups.google.com/d/forum/slim-announce" << std::endl;
		SLIM_OUTSTREAM << "slim-discuss mailing list: https://groups.google.com/d/forum/slim-discuss" << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "---------------------------------------------------------------------------------" << std::endl << std::endl;

		SLIM_OUTSTREAM << "SLiM is free software: you can redistribute it and/or modify it under the terms" << std::endl;
		SLIM_OUTSTREAM << "of the GNU General Public License as published by the Free Software Foundation," << std::endl;
		SLIM_OUTSTREAM << "either version 3 of the License, or (at your option) any later version." << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "SLiM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;" << std::endl;
		SLIM_OUTSTREAM << "without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR" << std::endl;
		SLIM_OUTSTREAM << "PURPOSE.  See the GNU General Public License for more details." << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "You should have received a copy of the GNU General Public License along with" << std::endl;
		SLIM_OUTSTREAM << "SLiM.  If not, see <http://www.gnu.org/licenses/>." << std::endl << std::endl;
		
		SLIM_OUTSTREAM << "---------------------------------------------------------------------------------" << std::endl << std::endl;
	}
	
	SLIM_OUTSTREAM << "usage: slim -version | -usage | -testEidos | -testSLiM |" << std::endl;
	SLIM_OUTSTREAM << "   [-seed <seed>] [-time] [-mem] [-Memhist] [-x] <script file>" << std::endl;
	
	if (p_print_full_usage)
	{
		SLIM_OUTSTREAM << std::endl;
		SLIM_OUTSTREAM << "   -v[ersion]       : print SLiM's version information" << std::endl;
		SLIM_OUTSTREAM << "   -u[sage]         : print command-line usage help" << std::endl;
		SLIM_OUTSTREAM << "   -testEidos | -te : run built-in self-diagnostic tests of Eidos" << std::endl;
		SLIM_OUTSTREAM << "   -testSLiM | -ts  : run built-in self-diagnostic tests of SLiM" << std::endl;
		SLIM_OUTSTREAM << std::endl;
		SLIM_OUTSTREAM << "   -s[eed] <seed>   : supply an initial random number seed for SLiM" << std::endl;
		SLIM_OUTSTREAM << "   -t[ime]          : print SLiM's total execution time (in user clock time)" << std::endl;
		SLIM_OUTSTREAM << "   -m[em]           : print SLiM's peak memory usage" << std::endl;
		SLIM_OUTSTREAM << "   -M[emhist]       : print a histogram of SLiM's memory usage" << std::endl;
		SLIM_OUTSTREAM << "   -x               : disable SLiM's runtime safety/consistency checks" << std::endl;
	}
	
	if (p_print_header || p_print_full_usage)
		SLIM_OUTSTREAM << std::endl;
	
	exit(0);
}

int main(int argc, char *argv[])
{
	// parse command-line arguments
	unsigned long int override_seed = 0;					// this is the type defined for seeds by gsl_rng_set()
	unsigned long int *override_seed_ptr = nullptr;			// by default, a seed is generated or supplied in the input file
	const char *input_file = nullptr;
	bool keep_time = false, keep_mem = false, keep_mem_hist = false, skip_checks = false;
	
	// command-line SLiM generally terminates rather than throwing
	gEidosTerminateThrows = false;
	
	if (argc == 1)
		PrintUsageAndDie(true, true);
	
	for (int arg_index = 1; arg_index < argc; ++arg_index)
	{
		const char *arg = argv[arg_index];
		
		// -seed <x> or -s <x>: override the default seed with the supplied seed value
		if (strcmp(arg, "-seed") == 0 || strcmp(arg, "-s") == 0)
		{
			if (++arg_index == argc)
				PrintUsageAndDie(false, true);
			
			override_seed = strtol(argv[arg_index], NULL, 10);
			override_seed_ptr = &override_seed;
			
			continue;
		}
		
		// -time or -t: take a time measurement and output it at the end of execution
		if (strcmp(arg, "-time") == 0 || strcmp(arg, "-t") == 0)
		{
			keep_time = true;
			
			continue;
		}
		
		// -mem or -m: take a peak memory usage measurement and output it at the end of execution
		if (strcmp(arg, "-mem") == 0 || strcmp(arg, "-m") == 0)
		{
			keep_mem = true;
			
			continue;
		}
		
		// -mem or -m: take a peak memory usage measurement and output it at the end of execution
		if (strcmp(arg, "-Memhist") == 0 || strcmp(arg, "-M") == 0)
		{
			keep_mem = true;		// implied by this
			keep_mem_hist = true;
			
			continue;
		}
		
		// -x: skip runtime checks for greater speed, or to avoid them if they are causing problems
		if (strcmp(arg, "-x") == 0)
		{
			skip_checks = true;
			eidos_do_memory_checks = false;
			
			continue;
		}
		
		// -version or -v: print version information
		if (strcmp(arg, "-version") == 0 || strcmp(arg, "-v") == 0)
		{
			SLIM_OUTSTREAM << "SLiM version 2.1, built " << __DATE__ << " " __TIME__ << std::endl;	// SLIM VERSION
			exit(0);
		}
		
		// -testEidos or -te: run Eidos tests and quit
		if (strcmp(arg, "-testEidos") == 0 || strcmp(arg, "-te") == 0)
		{
			gEidosTerminateThrows = true;
			Eidos_WarmUp();
			RunEidosTests();
			exit(0);
		}
		
		// -testSLiM or -ts: run SLiM tests and quit
		if (strcmp(arg, "-testSLiM") == 0 || strcmp(arg, "-ts") == 0)
		{
			gEidosTerminateThrows = true;
			Eidos_WarmUp();
			SLiM_WarmUp();
			RunSLiMTests();
			exit(0);
		}
		
		// -usage or -u: print usage information
		if (strcmp(arg, "-usage") == 0 || strcmp(arg, "-u") == 0 || strcmp(arg, "-?") == 0)
			PrintUsageAndDie(false, true);
		
		// this is the fall-through, which should be the input file, and should be the last argument given
		if (arg_index + 1 != argc)
			PrintUsageAndDie(false, true);
		
		input_file = argv[arg_index];
	}
	
	// check that we got what we need
	if (!input_file)
		PrintUsageAndDie(false, true);
	
	// announce if we are running a debug build or are skipping runtime checks
#ifdef DEBUG
	SLIM_ERRSTREAM << "// ********** DEBUG defined – you are not using a release build of SLiM" << std::endl << std::endl;
#endif
	if (skip_checks)
		SLIM_ERRSTREAM << "// ********** The -x command-line option has disabled some runtime checks" << std::endl << std::endl;
	
	// keep time (we do this whether or not the -time flag was passed)
	clock_t begin = clock();
	
	// keep memory usage information, if asked to
	size_t *mem_record = nullptr;
	int mem_record_index = 0;
	int mem_record_capacity = 0;
	size_t initial_mem_usage = 0;
	size_t peak_mem_usage = 0;
	
	if (keep_mem_hist)
	{
		mem_record_capacity = 16384;
		mem_record = (size_t *)malloc(mem_record_capacity * sizeof(size_t));
	}
	
	if (keep_mem)
	{
		// note we subtract the size of our memory-tracking buffer, here and below
		initial_mem_usage = EidosGetCurrentRSS() - mem_record_capacity * sizeof(size_t);
	}
	
	// run the simulation
	Eidos_WarmUp();
	SLiM_WarmUp();
	
	SLiMSim *sim = new SLiMSim(input_file);
	sim->InitializeRNGFromSeed(override_seed_ptr);
	
	if (keep_mem_hist)
		mem_record[mem_record_index++] = EidosGetCurrentRSS() - mem_record_capacity * sizeof(size_t);
	
	if (sim)
	{
#if DO_MEMORY_CHECKS
		// We check memory usage at the end of every 10 generations, to be able to provide the user with a decent error message
		// if the maximum memory limit is exceeded.  Every 10 generations is a compromise; these checks do take a little time.
		// Even with a model that runs through generations very quickly, though, checking every 10 makes little difference.
		// Models in which the generations take longer will see no measurable difference in runtime at all.  Note that these
		// checks can be disabled with the -x command-line option.
		int mem_check_counter = 0, mem_check_mod = 10;
#endif
		
		// Run the simulation to its natural end
		while (sim->RunOneGeneration())
		{
			if (keep_mem_hist)
			{
				if (mem_record_index == mem_record_capacity)
				{
					mem_record_capacity <<= 1;
					mem_record = (size_t *)realloc(mem_record, mem_record_capacity * sizeof(size_t));
				}
				
				mem_record[mem_record_index++] = EidosGetCurrentRSS() - mem_record_capacity * sizeof(size_t);
			}
			
#if DO_MEMORY_CHECKS
			if (eidos_do_memory_checks && ((++mem_check_counter) % mem_check_mod == 0))
			{
				// Check memory usage at the end of the generation, so we can print a decent error message
				std::ostringstream message;
				
				message << "(Limit exceeded at end of generation " << sim->Generation() << ".)" << std::endl;
				
				EidosCheckRSSAgainstMax("main()", message.str());
			}
#endif
		}
		
		// clean up; but this is an unnecessary waste of time in the command-line context
		//delete sim;
		//gsl_rng_free(gEidos_rng);
	}
	
	// end timing and print elapsed time
	clock_t end = clock();
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
	
	if (keep_time)
		SLIM_ERRSTREAM << "// ********** CPU time used: " << time_spent << std::endl;
	
	// print memory usage stats
	if (keep_mem)
	{
		peak_mem_usage = EidosGetPeakRSS();
		
		SLIM_ERRSTREAM << "// ********** Initial memory usage: " << initial_mem_usage << " bytes (" << initial_mem_usage / 1024.0 << "K, " << initial_mem_usage / (1024.0 * 1024) << "MB)" << std::endl;
		SLIM_ERRSTREAM << "// ********** Peak memory usage: " << peak_mem_usage << " bytes (" << peak_mem_usage / 1024.0 << "K, " << peak_mem_usage / (1024.0 * 1024) << "MB)" << std::endl;
	}
	
	if (keep_mem_hist)
	{
		SLIM_ERRSTREAM << "// ********** Memory usage history (execute in R for a plot): " << std::endl;
		SLIM_ERRSTREAM << "memhist <- c(" << std::endl;
		for (int hist_index = 0; hist_index < mem_record_index; ++hist_index)
		{
			SLIM_ERRSTREAM << "   " << mem_record[hist_index];
			if (hist_index < mem_record_index - 1)
				SLIM_ERRSTREAM << ",";
			SLIM_ERRSTREAM << std::endl;
		}
		SLIM_ERRSTREAM << ")" << std::endl;
		SLIM_ERRSTREAM << "initial_mem <- " << initial_mem_usage << std::endl;
		SLIM_ERRSTREAM << "peak_mem <- " << peak_mem_usage << std::endl;
		SLIM_ERRSTREAM << "#scale <- 1; scale_tag <- \"bytes\"" << std::endl;
		SLIM_ERRSTREAM << "#scale <- 1024; scale_tag <- \"K\"" << std::endl;
		SLIM_ERRSTREAM << "scale <- 1024 * 1024; scale_tag <- \"MB\"" << std::endl;
		SLIM_ERRSTREAM << "#scale <- 1024 * 1024 * 1024; scale_tag <- \"GB\"" << std::endl;
		SLIM_ERRSTREAM << "plot(memhist / scale, type=\"l\", ylab=paste0(\"Memory usage (\", scale_tag, \")\"), xlab=\"Generation (start)\", ylim=c(0,peak_mem/scale), lwd=4)" << std::endl;
		SLIM_ERRSTREAM << "abline(h=peak_mem/scale, col=\"red\")" << std::endl;
		SLIM_ERRSTREAM << "abline(h=initial_mem/scale, col=\"blue\")" << std::endl;
		
		free(mem_record);
	}
	
	return EXIT_SUCCESS;
}




































































