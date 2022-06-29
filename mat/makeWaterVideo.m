function makeWaterVideo()
%%
    fpath = uigetdir('/Users/kc32/Desktop/mnt/localtigress/myxoSim');    

    files = dir(fullfile(fpath,"water/"));
    del = [];

    for i = 1:numel(files)
        if (files(i).name(1) == '.') || ~contains(files(i).name,'0')
            del = [del; i];
        end
    end
    files(del) = [];

    lines = readlines(fullfile(fpath,'parameters.h'));

    bSstr = lines(contains(lines,'boxSize'));
    BS = regexp(bSstr,'\d+\.?\d*','match');
    boxSize = str2double(BS{1});

    v = VideoWriter(fullfile(fpath,'WaterVideo.mp4'),'MPEG-4');
    v.Quality = 95;
    v.FrameRate = 30;
    open(v);

    fig = figure('Units','pixels','Position',[0 0 400 400],'Color','w');
    ax = axes(fig,'Position',[0 0 1 1]);
    set(ax,'Visible','off','xlim',[0 boxSize],'ylim',[0 boxSize],'clim', [-0.5 1.0]);
    hold(ax,'on');
    
    for t = 1:numel(files)
        cla(ax);
        water = readmatrix(fullfile(files(t).folder,files(t).name));
        xs = water(:,1);
        ys = water(:,2);
        zs = water(:,3);

        p = plot3(xs,ys,zs,'.','MarkerSize',3);

        F = getframe;
        writeVideo(v, F.cdata);

    end
    close(v);
end