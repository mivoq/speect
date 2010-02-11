

%feature("shadow") SMap::__str__()
%{
    def __str__(self):
        num_features = len(self)
        if num_features > 0:
            stri = "{ " 
                  
            first = True
            count = num_features

            for ik in self:
                if not first:
                    stri += '\n    '
                first = False
                count -= 1
                if count != 0:
                    stri += '%15.15s => %s,' %(ik, repr(self[ik]))
                else:
                    stri += '%15.15s => %s' %(ik, repr(self[ik]))
                
            if  num_features > 0:
                stri += '    }\n'

        else:
            stri = "{}"
            
        return stri
%}    
