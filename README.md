# Driver-Vehicle-Info-Record
C Programming - Pointers and Structs

----------
4 STRUCTS: 

1. Driver
2. Vehicle
3. Infraction
4. PoliceDatabase

--------------
FUNCTIONS

1.registerDriver()
- takes a PoliceDatabase * and driver information (i.e., license, name, address, city, province) as parameters
- add the driver to the given database - assuming that the database is not full, otherwise the driver is not to be registered
- return 1 if the registration was ok, otherwise 0

2. registerVehicle()
- takes a PoliceDatabase * and vehicle information (i.e., make, model, year, color, plate and driver’s license) as parameters
- add the vehicle to the given database - assuming that the database is not full, otherwise the vehicle is not to be registered
- store the vehicle’s owner properly by finding the one with the given license
- return 1 if the registration was ok, otherwise 0

3. unregisterVehicle()
- takes a PoliceDatabase * and a vehicle plate as parameters
- remove the vehicle with that plate from the database without altering the order of the vehicles currently in the database and without leaving any gaps in the array
- copy all other vehicles back one position in the array to fill in the gap
- If the plate is not in the database, then nothing is to be done
- return 1 if the unregistration was ok, otherwise 0

4. pay()
- takes a single Infraction * as parameter
- pays the infraction

5. changeOwner()
- takes a PoliceDatabase *, a vehicle plate and a driver’s license as its parameters
- update the database by changing the owner information for the vehicle with the given plate to the driver with the given license
- If either the plate or the driver’s license is not in the database, then nothing is to be done
- return 1 if the change was made, otherwise 0

6. reportStolen()
- takes a PoliceDatabase * and a vehicle plate as its parameters
- properly record that the vehicle with the given plate has been stolen
- If the plate is not in the database, then nothing is to be done
- return 1 if the car was found, otherwise 0

7. issueInfraction()
- takes a PoliceDatabase *, a driver’s license and the infraction information as its parameters
- cause the driver with the given driver’s license to receive an infraction for the given amount and description on the given date 
- updating the database as necessary
- return the Infraction created
- If the number of infractions has reached its limit in the database, then nothing is to be done

8. hasOutstandingInfractions()
- takes a PoliceDatabase * and a Driver * as its parameters
- return 1 if the driver has any infractions that have not been paid yet, otherwise it should return 0

9. shouldStopVehicle()
- takes a PoliceDatabase * and a vehicle plate as its parameters
- make use of the function hasOutstandingInfractions()
- decide whether or not the police should pull this vehicle over
- return 1 if and only if the vehicle has been reported stolen or if the vehicle's owner has at least one outstanding infraction 
- otherwise, the function return 0

10. showInfractionsFor()
- takes a PoliceDatabase * and a driver’s license as its parameters
- display a list of all infractions that the driver with this license has incurred showing (on successive lines) the driver's infractions 
- and then finish with the total number of outstanding infractions

11. cleanDrivers()
- takes a PoliceDatabase * and a double star pointer (i.e., Driver **) to an initially-null array of drivers as its parameters
- determine the number of clean drivers (i.e., all drivers who have never had any infractions) 
- and then dynamically- allocate a Driver * array with that exact size and fill it in with drivers that are found to be clean
- set the incoming Driver ** parameter to the newly-allocated array

12. showInfractionReport()
- takes a PoliceDatabase * as its parameter
- list information about all drivers who have had at least one infraction
- Each line in the list shows a driver’s name followed by the number of infractions that they received and the total amount for all infractions that they have already paid



