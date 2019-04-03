require(["lib/common"], function () {
    require(["jquery"], function ($) {
        $(document).ajaxComplete(function (event, xhr, settings) {
            // Add summon list to support summon tiles
            if (window.location.href.includes("supporter") && settings.url.includes("supporter")) {
                $(".lis-supporter").each(function () {
                    var i = -1;
                    var map = function () {
                        i++;
                        var color;
                        if ($(this).attr("data-supporter-evolution") == "3") color = "#ffa826";
                        else if ($(this).attr("data-supporter-evolution") == "4") color = "#7f12b7";
                        else if ($(this).attr("data-supporter-evolution") == "5") color = "#07aeef";
                        else color = "transparent";
                        return $(this).find(".prt-summon-image img").clone()
                            .css({ "position": "absolute", "height": "11px", "right": "1px", "border-left": "1px solid " + color })
                            .css("top", function () { return `${i * 11 + 2}px`; })
                            .get(0);
                    };
                    var selector = `.lis-supporter[data-supporter-user-id=${$(this).attr("data-supporter-user-id")}]`;
                    $(this).append($(".prt-supporter-attribute:not(:first) " + selector).not(this).map(map))
                        .append($(".prt-supporter-attribute:first " + selector).not(this).map(map));
                });
            }
            // Show all summons on profile
            if (window.location.href.includes("#profile") && settings.url.includes("content")) {
                var map = function () {
                    var color;
                    if ($(this).has(".bless-rank1-style").length > 0) color = "#ffa826";
                    else if ($(this).has(".bless-rank2-style").length > 0) color = "#7f12b7";
                    else if ($(this).has(".bless-rank3-style").length > 0) color = "#07aeef";
                    else color = "transparent";
                    return $(this).find(".img-fix-summon").clone()
                        .css({ "width": "67px", "border-bottom": "2px solid " + color })
                        .get(0);
                };
                $("<div>", { "class": "prt-player" }).css({ "padding": "15px 25px" })
                    .append($(".prt-fix-support-wrap:not(:first) .prt-fix-support").map(map))
                    .append($(".prt-fix-support-wrap:first .prt-fix-support").map(map))
                    .insertAfter(".prt-status");
            }
        });
    });
});