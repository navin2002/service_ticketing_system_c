# ServiceTicketingSystem
## Description
Ticketing system for servicing of Hardware/software using Data Structures and C programming language. This Project uses Queue data structure in C.
In total there a 3 queues 
  -Hardware queue : holds nodes realted to hardware problems
  -Software queue: holds nodes realted to software problems
  -Service Log: holds nodes which have been serviced
 
 When an ticket is raised by a customer, it is added to the respective queues. When these tickets get resolved, they are added to the service log.

To try this project, compile and run the Service_Ticketing_system.c

Password for Hardware Engineer: hardware101
Password for Software Engineer: software202
Password for Admin: admin303

##Functions
Customer can
  -Raise Complaint
  -Remove Complaint
  -View details of a complaint with complaint ID
  -Check their position in the Queue
  -Modify details of a complaint with complaint ID
Software/Hardware Engineer can
  -View all pending services
  -View next in queue
  -View details of a complaint
  -Change status of current service
Admin can
  -View all pending services
  -View next in both queues
  -View details of an ongoing complaint
  -View service log
