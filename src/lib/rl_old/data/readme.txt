
archetypes.json : information for creating entities 
    e.g. a "creature" has "movement" and "asciirep" components
    
ecs.json        : specifies the script files to load for the ecs system
    -nt-
    
exemplars.json  : instantiated entities, invisible to the world, useful for quickly cloning entities
    e.g. a "rat_common" is an instance of a "creature" archetype, and we can clone many of these
    
instances.json  : instantiated entities that the application loads
    e.g. the player entity, the logger entity, the main window entity
    
queries.json    : complex, reusable queries to be used by the systems
    e.g. "tag player, cmp WizardClass"
    short version is "tag::blah" and "cmp::blah"
    
readme.txt      : this file
    -nt-
    
systems.json    : specification of all systems: type, name, queries, etc.
    e.g. "Logger" listens to queries "cmp_Log"