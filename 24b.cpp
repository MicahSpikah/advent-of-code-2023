// At time=t, my rock is at x + dx * t, y + dy * t, z + dz * t
// Are there possibly multiple solutions in one dimension? That seems so unlikely, there are only so many unknowns...
// Start by considering x only
/*
   r.x + r.dx * t0 == h0.x + h0.dx * t0   &&   r.y + r.dy * t0 == h0.y + h0.dy * t0   &&   r.z + r.dz * t0 == h0.z + h0.dz * t0
   (r.x - h0.x) == (h0.dx - r.dx) * t0
   (r.x - h1.x) == (h1.dx - r.dx) * t1
   (r.x - h2.x) == (h2.dx - r.dx) * t2
   ...
// Hm, there's a new t unknown for each hailstone
// What do I know? Assuming my dx is positive, I must be faster than everyone to my right and slower than everyone to my left
// So what does that mean? If I sort by hail dx, r.dx will be in a spot where there exists an r.x such that h.x < r.x iff r.dx < h.dx
// So let's argument by contradiction...
// Assume that r.dx < h1.dx. That means that h1.x < r.x. BUT if there exists h1.dx < h2.dx 

// e.g. Theres a h.x of 127786384216516 and also 258357201611452 both with a dx of 150. So all r.x between those numbers are eliminated
        With similar manual logic, I can eliminate between: 
        r.dx=-76 or -78???
        r.dy=284...242
        r.dz=72 or 71 or 70
        if dz is 72 then z is 231185943543128
              is 71 then z is 231363386790708
              is 70 then z is 231363386790708...232010796540278
              reminder than t_n = (r.z - h.z) / (h.dz - r.dz)
              I tested dz of 71 and ALL intersections are at integer timestamps which wasn't technically part of the problem but
                seems QUITE suspicious. Let's assume that. And now I know all the timestamps!
              I then tested dx of -76 in excel and it didn't quite line up. Let's try -78...
                318090941338468 fits PERFECTLY (which I found by binary searching in macros)
              I then just picked a couple random hailstones and solved for y! Perfect! My sketch ended up solving everything without compiling any code :)
   */

//TODO: I COULD write this out as an algorithm but it's 12:30 am. Maybe someday.
