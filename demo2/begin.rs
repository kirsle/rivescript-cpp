! version = 2.0

/* multi
line
comment */

> begin
	+ request // This trigger is tested first.
	- {ok}    // An {ok} in the response means it's okay to get a real reply
< begin

// The Botmaster's Name
! var master = localuser

// Bot Variables
! var name     = Aiden
! var fullname = Aiden Rive
! var age      = 5
! var birthday = October 12
! var sex      = male
! var location = Michigan
! var city     = Detroit
! var eyes     = blue
! var hair     = light brown
! var hairlen  = short
! var color    = blue
! var band     = Nickelback
! var book     = Myst
! var author   = Stephen King
! var job      = robot
! var website  = www.rivescript.com

// Substitutions
! sub &quot;    = "
! sub &apos;    = '
! sub &amp;     = &
! sub &lt;      = <
! sub &gt;      = >
! sub +         = plus
! sub -         = minus
! sub /         = divided
! sub *         = times
! sub i'm       = i am
! sub i'd       = i would
! sub i've      = i have
! sub i'll      = i will
! sub don't     = do not
! sub isn't     = is not
! sub you'd     = you would
! sub you're    = you are
! sub you've    = you have
! sub you'll    = you will
! sub he'd      = he would
! sub he's      = he is
! sub he'll     = he will
! sub she'd     = she would
! sub she's     = she is
! sub she'll    = she will
! sub they'd    = they would
! sub they're   = they are
! sub they've   = they have
! sub they'll   = they will
! sub we'd      = we would
! sub we're     = we are
! sub we've     = we have
! sub we'll     = we will
! sub whats     = what is
! sub what's    = what is
! sub what're   = what are
! sub what've   = what have
! sub what'll   = what will
! sub can't     = can not
! sub whos      = who is
! sub who's     = who is
! sub who'd     = who would
! sub who'll    = who will
! sub don't     = do not
! sub didn't    = did not
! sub it's      = it is
! sub could've  = could have
! sub couldn't  = could not
! sub should've = should have
! sub shouldn't = should not
! sub would've  = would have
! sub wouldn't  = would not
! sub when's    = when is
! sub when're   = when are
! sub when'd    = when did
! sub y         = why
! sub u         = you
! sub ur        = your
! sub r         = are
! sub n         = and
! sub im        = i am
! sub wat       = what
! sub wats      = what is
! sub ohh       = oh
! sub becuse    = because
! sub becasue   = because
! sub becuase   = because
! sub practise  = practice
! sub its a     = it is a
! sub fav       = favorite
! sub fave      = favorite
! sub yesi      = yes i
! sub yetit     = yet it
! sub iam       = i am
! sub welli     = well i
! sub wellit    = well it
! sub amfine    = am fine
! sub aman      = am an
! sub amon      = am on
! sub amnot     = am not
! sub realy     = really
! sub iamusing  = i am using
! sub amleaving = am leaving
! sub yuo       = you
! sub youre     = you are
! sub didnt     = did not
! sub ain't     = is not
! sub aint      = is not
! sub wanna     = want to
! sub brb       = be right back
! sub bbl       = be back later
! sub gtg       = got to go
! sub g2g       = got to go
! sub lyl       = love you lots
! sub gf        = girlfriend
! sub g/f       = girlfriend
! sub bf        = boyfriend
! sub b/f       = boyfriend
! sub b/f/f     = best friend forever
! sub :-)       = smile
! sub :)        = smile
! sub :d        = grin
! sub :-d       = grin
! sub :-p       = tongue
! sub :p        = tongue
! sub ;-)       = wink
! sub ;)        = wink
! sub :-(       = sad
! sub :(        = sad
! sub :'(       = cry
! sub :-[       = shy
! sub :-\       = uncertain
! sub :-/       = uncertain
! sub :-s       = uncertain
! sub 8-)       = cool
! sub 8)        = cool
! sub :-*       = kissyface
! sub :-!       = foot
! sub o:-)      = angel
! sub >:o       = angry
! sub :@        = angry
! sub 8o|       = angry
! sub :$        = blush
! sub :-$       = blush
! sub :-[       = blush
! sub :[        = bat
! sub (a)       = angel
! sub (h)       = cool
! sub 8-|       = nerdy
! sub |-)       = tired
! sub +o(       = ill
! sub *-)       = uncertain
! sub ^o)       = raised eyebrow
! sub (6)       = devil
! sub (l)       = love
! sub (u)       = broken heart
! sub (k)       = kissyface
! sub (f)       = rose
! sub (w)       = wilted rose

// Person substitutions
! person i am    = you are
! person you are = I am
! person i'm     = you're
! person you're  = I'm
! person my      = your
! person your    = my
! person you     = I
! person i       = you

// Set arrays
! array malenoun   = male guy boy dude boi man men gentleman gentlemen
! array femalenoun = female girl chick woman women lady babe
! array mennoun    = males guys boys dudes bois men gentlemen
! array womennoun  = females girls chicks women ladies babes
! array lol        = lol lmao rofl rotfl haha hahaha
! array colors     = white black orange red blue green yellow cyan fuchsia gray grey brown turquoise pink purple gold silver navy
! array height     = tall long wide thick
! array measure    = inch in centimeter cm millimeter mm meter m inches centimeters millimeters meters
! array yes        = yes yeah yep yup ya yea
! array no         = no nah nope nay
