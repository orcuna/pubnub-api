(function(){

function now() {return+new Date}

// ----------------------------------------------------------------------
// PUBLISH A MESSAGE (SEND)
// ----------------------------------------------------------------------
var net = PUBNUB.init({
    publish_key   : 'demo',
    subscribe_key :'demo'
})
,   channel        = 'performance-meter-' + now() + Math.random()
,   start          = now()
,   sent           = 0
,   mps_avg        = 0
,   receiving      = 0
,   median_display = {}
,   lat_avg        = 0
,   median         = [0]
,   publish        = (function(){
    var sendqueue = []
    ,   sending   = 0;

    function deliver() {
        //if (receiving) return;
        receiving = 1;

        if (sendqueue.length) net.publish(sendqueue.pop())
        else                  sending = 0;
    }

    return function( message, callback ) {
        if (!net) return;

        start = now();

        sendqueue = [];
        sendqueue.push({
            channel  : channel,
            message  : message || 1,
            callback : function(info) {
                info && info[0] || publish( message, callback );
                deliver();
                callback && callback(info);
            }
        });
        if (!sending) { sending = 1; deliver(); }
    };
})();

// ----------------------------------------------------------------------
// SUBSCRIBE FOR MESSAGES (RECEIVE)
// ----------------------------------------------------------------------
net.subscribe({
    channel   : channel,
    connect   : publish,
    reconnect : publish,
    callback  : function( msg, envelope ) {
        var latency     = (now() - start) || median[1]
        ,   new_mps_avg = 1000 / latency;

        sent++;
        receiving = 0;

        lat_avg = (latency + lat_avg) / 2;
        mps_avg = (new_mps_avg + mps_avg) / 2;
        median.push(latency);

        start = now();
        publish();
    }
});
publish();
setInterval( function() { set_rps(mps_avg) }, 500 );

// ----------------------------------------------------------------------
// CALCULATE MEDIAN VALUES
// ----------------------------------------------------------------------
var median_template = PUBNUB.$('median-template').innerHTML
,   median_out      = PUBNUB.$('performance-medians');

function update_medians() {
    var length = median.length - 1
    ,   medlen = Math.floor(length/2);

    function get_median(val) {
        return median[medlen + Math.floor(length * val)];
    }
    function get_median_low(val) {
        return median[Math.floor(medlen * val)||1];
    }

    median = median.sort(function(a,b){return a-b});

    median_display = {
        '1'   : median[1],

        '2'   : get_median_low(0.02),
        '5'   : get_median_low(0.05),
        '10'  : get_median_low(0.1),
        '20'  : get_median_low(0.2),
        '25'  : get_median_low(0.5),
        '30'  : get_median_low(0.6),
        '40'  : get_median_low(0.8),
        '45'  : get_median_low(0.9),

        '50'  : median[medlen],

        '66'  : get_median(0.16),
        '75'  : get_median(0.25),
        '80'  : get_median(0.30),
        '90'  : get_median(0.40),
        '95'  : get_median(0.45),
        '98'  : get_median(0.48),
        '99'  : get_median(0.49),

        '100' : median[length-1]
    };

    median_out.innerHTML = PUBNUB.supplant(
        median_template,
        median_display
    );
}
update_medians();

// ----------------------------------------------------------------------
// DISPLAY TOTAL MESSAGES SENT/RECEIVED
// ----------------------------------------------------------------------
var performance_sent_template = PUBNUB.$('messages-sent-template').innerHTML
,   performance_sent          = PUBNUB.$('performance-sent');

function update_messages_received() {
    performance_sent.innerHTML = PUBNUB.supplant(
        performance_sent_template, {
            sent : median.length * 2
        }
    );
}

// ----------------------------------------------------------------------
// SET RPS FOR DISPLAY
// ----------------------------------------------------------------------
var set_rps = (function() {
    var rps   = PUBNUB.$('performance-meter-number')
    ,   arrow = PUBNUB.$("performance-arrow");

    return function (val) {
        var meter = -90.0 + ((val || 0)*10);
        animate( arrow, [ { d : 0.5, r : meter > 90 ? 90 : meter } ] );
        rps.innerHTML = ''+Math.ceil(val);
        update_medians();
        draw_graph();
        update_messages_received();
    };
})();

// ----------------------------------------------------------------------
// GRAPH DISPLAY
// ----------------------------------------------------------------------
var draw_graph = (function(){
    var graph    = PUBNUB.$('performance-graph').getContext("2d")
    ,   height   = 50
    ,   barwidth = 20
    ,   bargap   = 20
    ,   modrend  = 3
    ,   barscale = 8
    ,   position = 0
    ,   bgcolor  = "#dfd6b9"
    ,   fgcolor  = "#f2efe3";

    // Graph Gradient
    var gradient = graph.createLinearGradient( 0, 0, 0, height * 1.5 );
    gradient.addColorStop( 0, fgcolor );
    gradient.addColorStop( 1, bgcolor );

    return function(values) {
        // Rate Limit Canvas Painting
        if (!(sent % modrend)) return;

        // Clear
        position = 0;
        graph.fillStyle = bgcolor;
        graph.fillStyle = fgcolor;
        graph.fillRect( 0, 0, 640, height );

        // Lines
        graph.fillStyle = gradient;
        PUBNUB.each( median_display, function( key, latency ) {
            latency = latency / barscale;
            latency = latency > height ? height : latency;

            graph.fillRect(
                position * (barwidth + bargap) + (bargap / 2),
                height - latency,
                barwidth,
                height
            );

            position++;
        } );
    };
})();

})();
