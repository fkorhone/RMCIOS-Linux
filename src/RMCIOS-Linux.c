/* 
RMCIOS - Reactive Multipurpose Control Input Output System
Copyright (c) 2018 Frans Korhonen

RMCIOS was originally developed at Institute for Atmospheric 
and Earth System Research / Physics, Faculty of Science, 
University of Helsinki, Finland

Assistance, experience and feedback from following persons have been 
critical for development of RMCIOS: Erkki Siivola, Juha Kangasluoma, 
Lauri Ahonen, Ella Häkkinen, Pasi Aalto, Joonas Enroth, Runlong Cai, 
Markku Kulmala and Tuukka Petäjä.

This file is part of RMCIOS. This notice was encoded using utf-8.

RMCIOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RMCIOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public Licenses
along with RMCIOS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "RMCIOS-system.h"

int stdin_id=0 ;
void init_gnu_channels(const struct context_rmcios *context) ; 

// Channel for setting stdinput destination. And writing to stdout
void stdio_class_func(void *this, 
                      const struct context_rmcios *context, int id, 
                      enum function_rmcios function,
                      enum type_rmcios paramtype,
                      union param_rmcios returnv, 
                      int num_params,union param_rmcios param)
{
	int plen ;
	const char *s; 
	switch(function)
	{
		case write_rmcios: 
			if(num_params<1) break ;
			// Determine the needed buffer size
			plen= param_string_alloc_size(context,paramtype,param, 0) ; 
			{
				char buffer[plen] ; // allocate buffer
				s=param_to_string(context,paramtype,param, 0, plen, buffer) ;
				fwrite(s,strlen(s),1,stdout); 
			}
			break ;
	}
}


//////////////////////////////////////////////////////////////////////////
// Channel for printout         
//////////////////////////////////////////////////////////////////////////
void stdout_func (void *data,
                  const struct context_rmcios *context, int id,
                  enum function_rmcios function,
                  enum type_rmcios paramtype,
                  union param_rmcios returnv,
                  int num_params, const union param_rmcios param)
{
   switch (function)
   {
   case write_rmcios:
      if (num_params < 1)
         break;
      int blen = param_string_alloc_size (context, paramtype, param, 0);
      {
         char buffer[blen];
         const char *str;
         str = param_to_string (context, paramtype, param, 0, blen, buffer);
         puts (str);
      }
      break;
   }
}

// Channel function for allocating and freeing memory
void mem_func (void *data,
               const struct context_rmcios *context, int id,
               enum function_rmcios function,
               enum type_rmcios paramtype,
               union param_rmcios returnv,
               int num_params, const union param_rmcios param)
{
   switch (function)
   {
   case help_rmcios:
      // MEMORY INTERFACE: 
      return_string (context, paramtype, returnv,
                     " read mem \r\n -read ammount of free memory\r\n"
                     " write mem \r\n -read memory allocation block size\r\n"
                     " write mem n_bytes \r\n Allocate n bytes of memory\r\n"
                     "   -Returns address of the allocated memory\r\n"
                     "   -On n_bytes < 0 allocates complete allocation blocks\r\n"
                     "   -returns 0 length on failure\r\n"
                     " write mem (empty) addr(buffer/id)  \r\n"
                     " -free memory pointed by addr in buffer\r\n"
                     );
      break;

   case read_rmcios:
      if (num_params == 0)      // read ammount of free memory
         if (num_params > 0)    //read memory block by remote access id
            if (num_params > 1) // +size bytes to read
               if (num_params > 2)      // +offset to start reading from\r\n");
                  break;

   case write_rmcios:
      if (num_params == 0)
      {
      } // Read memory allocation block size
      if (num_params == 1)      // Allocate n bytes of memory
      {
         void *ptr = malloc (param_to_integer (context, paramtype,
                                               (const union param_rmcios)
                                               param, 0));
         return_binary (context, paramtype, returnv, (char *) &ptr,
                        sizeof (ptr));
      }
      if (num_params > 1)
      {
      } // Write data to memory by access id
      if (num_params > 2)
      {
      } // +max size in bytes
      if (num_params > 3)
      {
      } // +starting at offset
      if (num_params == 2)      // Free 
      {
         if (param_to_integer
             (context, paramtype, (const union param_rmcios) param, 0) == 0)
         {
            char *ptr = NULL;
            param_to_binary (context, paramtype, param, 1,
                             sizeof (ptr), (char *) &ptr);
            if (ptr != NULL)
               free (ptr);
         }
      }
      break;
   }
}

int main(int argc, char *argv[])
{
	// Command line parameters:
	const char *init_filename ;
	init_filename="conf.ini" ;
	if(argc>1) init_filename=argv[1] ;

	printf("\nRMCIOS for Linux, Copyright (c) 2018 Frans Korhonen\n");
    printf("[" VERSION_STR "] \n");
	printf("\nInitializing system:\n");

	//////////////////////////////////////////////
	// Init channel system
	//////////////////////////////////////////////
    // data_handle_name,MAX_CLASSES,MAX_CHANNELS
	CREATE_STATIC_CHANNEL_SYSTEM_DATA(ch_sys_dat,100,500) ; 
	
    // init channel api system
	set_channel_system_data( (struct ch_system_data *)&ch_sys_dat) ;
	const struct context_rmcios *context = get_rmios_context() ; 

	///////////////////////////////////////////////
	/// Load modules
	///////////////////////////////////////////////
	init_base_channels(context) ;
    init_std_channels(context) ;
	init_gnu_channels(context) ;
	
	///////////////////////////////////////////////
	// stdio channel
	///////////////////////////////////////////////
	stdin_id=create_channel_str(context, "stdio",
                                (class_rmcios) stdio_class_func, 0);
	write_str(context, context->control , "link stdio control\r\n" ,0) ;

	//////////////////////////////////////////////////
	// channel help readout
	//////////////////////////////////////////////////
	if(argc>2 && strcmp("help",argv[1])==0)     
	{	
        init_filename=argv[1] ;
		write_str(context, linked_channels(context,stdin_id), 
                  "help ", stdin_id) ;
		write_str(context, linked_channels(context,stdin_id), 
                  argv[2], stdin_id) ;
		write_str(context, linked_channels(context,stdin_id), 
                  "\n", stdin_id) ;
		return 1;
	}

	////////////////////////////////////////////////////////////
	// initial configuration 
	/////////////////////////////////////////////////////////////
	FILE *fconf ;
	printf("Executing: %s\r\n",init_filename) ;
	write_str(context, context->control , "read as control file " ,0) ;
	write_str(context, context->control , init_filename ,0) ;
	write_str(context, context->control , "\r\n", 0) ;

	///////////////////////////////////////////////////////
	// configuration logging (conf.log)
	//////////////////////////////////////////////////////
	char c ;
	printf("Logging configuration: conf.log\r\n") ;
	FILE *conflog= fopen("conf.log","a") ; // Log the configuration
	if(conflog!=NULL)
	{
		char buffer[256] ;
		buffer[0]=0;
		
        read_str(context, channel_enum(context,"rtc_str"), buffer, 256) ;
		fprintf(conflog,"\n\n# RESTART at %s ########\n", buffer) ;

		// Local filesystem
		fconf= fopen(init_filename,"r") ; // Log the configuration
		if(fconf!=NULL) 
		{
			fprintf(conflog,"# Config from /local/conf.ini:\n") ;
			while( (c=fgetc(fconf))!=EOF) fputc(c,conflog) ;
			fclose(fconf) ; 
		}
		else printf("Could not open for reading: %s\r\n", init_filename) ;
		fclose(conflog) ; // Log the configuration
	}
	else printf("Could not open for appending: conf.log \r\n") ;

	printf("\r\nSystem initialized!\r\n") ;

	////////////////////////////////////////////////////////////////////
	// reception loop
	////////////////////////////////////////////////////////////////////
	char s[2] ;
	s[1]=0 ;
	while(1)
	{
        int ch=getchar();
        if(ch>0)
        {
		    s[0]=ch ;
		    s[1]=0 ;
		    write_str(context, linked_channels(context,stdin_id), s, stdin_id) ;
        }
	}
}


