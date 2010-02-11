

%feature("shadow") SUtterance::__str__()
%{
    def __str__(self):
        stri = "Utterance:\n"
        for f in self.features:
            stri += '    Feature: %20.20s => %s\n' %(f, repr(self.features[f]))

        for r in self:
            stri += self.relation_get(r).to_string(prefix="        ")
                       
        return stri
%}    
