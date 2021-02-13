// IOCTLS.H -- IOCTL code definitions for BuckDbg driver

#ifndef IOCTLS_H
#define IOCTLS_H

#ifndef CTL_CODE
	#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

#define MAX_TRANS_SIZE 65536


#define IOCTL_READ CTL_CODE( FILE_DEVICE_UNKNOWN,  \
						     0x800, 			   \
							 METHOD_BUFFERED,	   \
							 FILE_ANY_ACCESS	   \
						   )

#define IOCTL_WRITE CTL_CODE( FILE_DEVICE_UNKNOWN, 	\
						      0x801, 				\
							  METHOD_BUFFERED, 		\
							  FILE_ANY_ACCESS		\
							)

#define IOCTL_HOLD_8051 CTL_CODE( FILE_DEVICE_UNKNOWN, 	\
							0x805, 				\
							METHOD_BUFFERED, 		\
							FILE_ANY_ACCESS		\
							)

#define IOCTL_RUN_8051 CTL_CODE( FILE_DEVICE_UNKNOWN, 	\
							0x806, 				\
							METHOD_BUFFERED, 		\
							FILE_ANY_ACCESS		\
							)

#endif
