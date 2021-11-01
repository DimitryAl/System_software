require 'monitor'
require 'thread'


def generateNumber(x)
    t = rand($phone_array.length)
    while (t == x) || (t == 0)
        t = rand($phone_array.length)
    end
    return t
end


class Interface 

    attr_accessor :name, 
                  :phone,
                  :number

    def Wait(x)
        sleep(x)
    end

    def MakeCall(x)    #сделать звонок
        n = generateNumber(x)
        time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
        puts time + "\t" + @name + ' is calling to ' + $names[n]
        if $phone_array[n] == 0
            $phone_array[n] += 1
            if @name == 'Polyeuctus'        #тот, кому позвонил Полуэкт, получает подтверждение
                $confirmation[n] = true
            end
            if $confirmation[n] == true
                $confirmation[x] = true
            end
            return true, n
        else
            $phone_array[x] -= 1
            return false, -1
        end
    end

end

class Caller < Interface
    include MonitorMixin
    
    def initialize(name, number)
        @name = name
        @number = number
        @smth_test = new_cond
    end

    def polyeuctus()
        while true  
            
            mon_synchronize do
                if $phone_array[@number] != 0
                    @smth_test.wait
                end
                $phone_array[@number] += 1 
                
            end         
            res = MakeCall(@number)   
            if res[0] == true
                
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(2)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                
                $phone_array[res[1]] -= 1
                $phone_array[@number] -= 1

                mon_synchronize do 
                    puts "broadcast here"
                    @smth_test.signal
                end
                break

            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end    
    end


    def mother
        while true 
            mon_synchronize do
                if $phone_array[@number] != 0
                    puts 'talking with someone'
                    @smth_test.wait()
                    puts ' stop talking with someone'
                end
                $phone_array[@number] += 1
            end
            if $confirmation[@number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' got confirmation'
                $phone_array[@number] -= 1
                break
            end  
            res = MakeCall(@number)  
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                
                $phone_array[res[1]] -= 1

                mon_synchronize do 
                $phone_array[@number] -= 1
                @smth_test.broadcast
                end
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end
    end

    def girlfriend
        while true  
            self.mon_synchronize do
                if $phone_array[@number] != 0
                    puts @name + 'talking with someone'
                    @smth.wait
                    puts @name + 'stop talking with someone'
                end
                $phone_array[@number] += 1
           end
            if $confirmation[@number] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' got confirmation'
                $phone_array[@number] -= 1
                break
            end 
            res = MakeCall(@number)
            if res[0] == true
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' successfully called to ' + $names[res[1]]
                Wait(5)
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + @name + ' finished call'
                $phone_array[res[1]] -= 1
                $phone_array[@number] -= 1 
                @smth.broadcast
            else
                time = Time.new.hour.to_s + ':' + Time.new.min.to_s + ':' + Time.new.sec.to_s
                puts time + "\t" + 'Call failed, ' + @name + ' waits'
                Wait(2)
            end
        end
    end


end



girlfriend_number = 0   # кол-во девушек
granny_number = 0       # кол-во бабушек
n = 2 + girlfriend_number + granny_number                   

$confirmation = [false]*n
$confirmation[0] = true
$names = []*n
$names[0] = 'Polyeuctus'
$names[1] = 'Mother'
$phone_array = [0]*n
# for i in (1..n)
#     $phone_array << 0
# end

threads = []
threads << Thread.new {
    p = Caller.new('Polyeuctus', 0)
    p.polyeuctus
}
threads << Thread.new {
    m = Caller.new('Mother', 1)
    m.mother
}

# girlfriend_number.times do |i|
#      $names[i+2] = 'Girlfriend_' + (i+1).to_s
#      threads << Thread.new {
#         g = Caller.new('Girlfriend_' + (i+1).to_s, i+2)
#         g.girlfriend
#     }
# end

# granny_number.times do |i|
#     $names[i+2+girlfriend_number] = 'Granny_' + (i+1).to_s
#     threads << Thread.new {Grandmother.new('Granny_' + (i+1).to_s, $phone_array[i+2+girlfriend_number], i+2+girlfriend_number, n)}
# end

threads.each { |thread| thread.join}