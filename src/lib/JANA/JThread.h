//
//    File: JThread.h
// Created: Wed Oct 11 22:51:22 EDT 2017
// Creator: davidl (on Darwin harriet 15.6.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Jefferson Science Associates LLC Copyright Notice:  
// Copyright 251 2014 Jefferson Science Associates LLC All Rights Reserved. Redistribution
// and use in source and binary forms, with or without modification, are permitted as a
// licensed user provided that the following conditions are met:  
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.  
// 3. The name of the author may not be used to endorse or promote products derived
//    from this software without specific prior written permission.  
// This material resulted from work developed under a United States Government Contract.
// The Government retains a paid-up, nonexclusive, irrevocable worldwide license in such
// copyrighted data to reproduce, distribute copies to the public, prepare derivative works,
// perform publicly and display publicly and to permit others to do so.   
// THIS SOFTWARE IS PROVIDED BY JEFFERSON SCIENCE ASSOCIATES LLC "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
// JEFFERSON SCIENCE ASSOCIATES, LLC OR THE U.S. GOVERNMENT BE LIABLE TO LICENSEE OR ANY
// THIRD PARTES FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
// Description:
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#ifndef _JThread_h_
#define _JThread_h_

#include <thread>

#include <JApplication.h>

class JThread{
public:
	
		enum RUN_STATE_t {
			kRUN_STATE_INITIALIZING,
			kRUN_STATE_IDLE,
			kRUN_STATE_RUNNING,
			kRUN_STATE_ENDED,
			kRUN_STATE_OTHER
		};
	
		JThread(JApplication *app=NULL);
		virtual ~JThread();

		uint64_t GetNumEventsProcessed(void);
		void GetNumEventsProcessed(map<string,uint64_t> &Nevents);
		std::thread* GetThread(void);
		void Join(void);

		void End(void);
		bool IsIdle(void);
		bool IsEnded(void);
		bool IsJoined(void);
		void Loop(void);
		void Run(void);
		void SetQueues(const vector<JQueue*> *queues=NULL);
		void Stop(bool wait_until_idle = false);
		
protected:
		
		std::thread *_thread;
		RUN_STATE_t _run_state;           ///< Current state
		RUN_STATE_t _run_state_target;    ///< State to transtion to after current event
		bool _isjoined;
		JApplication *_japp;
		map<std::string, uint64_t> _events_processed;
		vector<JQueue*> _queues;
		
	private:

};

extern thread_local JThread *JTHREAD;

#endif // _JThread_h_

