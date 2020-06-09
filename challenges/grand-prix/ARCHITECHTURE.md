# Grand Prix

Multithreaded Grand Prix Simulator

## Architecture
Each competitor in the race is simulated as a separated `thread`. The competitors threads communicate with the main thread, which coordinates them all and controls the flow of the race. Memory's shared by communicating the competitors with the main goroutine, since all the competitors use and evaluate the same competition track. All the competitors send their movement requests through the same `channel`, and the main goroutine evaluate the request and send an answer through each competitor's `response channel`. 


## Competitors composition
Each comptetitor has several components that change over time in order to simulate the competitor's movement.

* **Location struct**: A competitor sends through the requests channel a new Location request, which refers to where the competitor wants to go. The Location struct contains the following elements
	* Competitor's ID
	* Competitor's desired rail
	* Competitor's desired position
	* Competitor's current lap

* **Max Speed**: It refers to the maximum speed that the competitor can have during the race.
* **Acceleration rate**: It refers to the rate that emulates the competitor's velocity increment.
* **Request channel**: It refers to the channel shared by all competitors, through which new Location requests are sent.
* **Response channel**: It refers to the booleans channel that receives the main goroutine's response. 
* **Deacceleration rate for racer**: It refers to the breaking rate when a competitor aproximmates to another competitor.
* **Desacceleration rate for curve**: It refers to the breaking rate when the competitor travels through a curve on the track. 

## GUI Update
Once a competitor's request is approved by the main goroutine, that competitor sends an update to the `prints goroutine` through the `updates channel`. The prints goroutine empties the updates channel, fills a slice where the last update of each competitor is stored and prints each competitor stats and the competition track updated with each competitor new position.

