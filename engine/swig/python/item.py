

%feature("shadow") SItem::__str__()
%{
    def __str__(self):
        return self.to_string()
%}    

%feature("shadow") SItem::to_string(const char *, const char *)
%{
    def to_string(self, prefix="", label="Item"):
        label_spacing = ''
        for s in range(0, len(label)):
            label_spacing += ' '

        num_features = len(self)
        if num_features > 0:
            stri = "%s%s: [ " %(prefix,label)
                  
            first = True
            count = num_features

            for ik in self:
                if not first:
                    stri += '\n%s%s    ' %(prefix, label_spacing)
                first = False
                count -= 1
                if count != 0:
                    stri += '%15.15s => %s,' %(ik, repr(self[ik]))
                else:
                    stri += '%15.15s => %s' %(ik, repr(self[ik]))
                
            if  num_features > 0:
                stri += '    ]\n'

        else:
            stri = ""
            
        i = self.daughter()
        n = 0
        daughter_prefix = '%s    ' %prefix
        while i:
            stri += '%s' %i.to_string(prefix=daughter_prefix, 
                                     label='Daughter')
            n += 1
            i = self.daughter(n)

        return stri
%}    
